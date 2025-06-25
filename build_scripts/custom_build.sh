#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"
cd ..

valid_languages=("C" "CPP")
valid_compilers=("g++" "clang++" "msvc")
valid_targets=("Linux" "Windows" "Mac")
valid_configs=("Debug" "Release" "RelWithDebInfo" "MinSizeRel")
existing_projects=()
existing_tests=()
header_exts=()
source_exts=()

mode="none"

projects=()
tests=()
language="none"
version="none"
compiler="clang++"
config="Debug"
target="Linux"
is_build="false"
is_all_project="false"

previous_project="none"

#########

set_file_extensions() {
if [[ "$language" == "C" ]]; then
	header_exts=("h")
	source_exts=("c")
elif [[ "$language" == "CPP" ]]; then
	header_exts=("h" "hpp")
	source_exts=("c" "cpp")
fi
}

collect_projects() {
	local project_paths=()
	mapfile -t project_paths < <(find "./projects/" -mindepth 1 -maxdepth 1 -type d)

	for project_path in "${project_paths[@]}"; do 
		local project_path_ext="$(basename "$project_path")"
		local project_name="${project_path_ext%.*}"
		existing_projects+=("$project_name")
	done
}

collect_tests() {
	local test_paths=()
	for project in "${existing_projects[@]}"; do
		if [[ ! -d "./projects/$project/tests/" ]]; then
			continue
		fi
		for ext in "${source_exts[@]}"; do
			mapfile -t test_paths < <(find "./projects/$project/tests/" -mindepth 1 -maxdepth 1 -type f -name "*.$ext")

			for test_path in "${test_paths[@]}"; do
				local test_path_ext="$(basename "$test_path")"
				local test_name="${test_path_ext%.*}"
				existing_tests+=("$project/$test_name")
			done
		done
	done
}

add_all_tests_for_project() {
	local project_name="$1"

	if [[ ! -d "./projects/$project_name/tests/" ]]; then
		echo "trying to add tests for invalid project: $project_name" >&2
		exit 1
	fi
	local test_paths=()
	for ext in "${source_exts[@]}"; do
		mapfile -t test_paths < <(find "./projects/$project_name/tests/" -mindepth 1 -maxdepth 1 -type f -name "*.$ext")

		for test_path in "${test_paths[@]}"; do
			local test_path_ext="$(basename "$test_path")"
			local test_name="${test_path_ext%.*}"
			tests+=("$project_name/$test_name")
		done

	done

}

contains_element() {
    local target="$1"
    shift
    local array=("$@")

    for element in "${array[@]}"; do
        if [[ "$element" == "$target" ]]; then
            return 0  
        fi
    done
    return 1  
}

##########



for arg in "$@"; do
	case "$arg" in
		"--all")
			case "$mode" in
				"project")
					is_all_project="true"
					;;
				"test")
					add_all_tests_for_project "$previous_project"
					;;
				*)
					echo "--all has to come after -project or -tests" >&2
					exit 1
					;;
			esac
			;;
		"--build")
			is_build="true"
			;;
		"-language")
			mode="language"
			;;
		"-project")
			mode="project"
			;;
		"-tests")
			mode="test"
			;;
		"-compiler")
			mode="compiler"
			;;
		"-config")
			mode="config"
			;;
		"-target")
			mode="target"
			;;
		*)

			case "$mode" in
				"language")
					language="$arg"
					set_file_extensions 
					collect_projects
					collect_tests
					;;
				"project")
					previous_project="$arg"
					projects+=("$arg")
					;;
				"test")
					if [[ "$previous_project" == "none" ]]; then 
						echo "tests must come after a project" >&2
						exit 1
					fi
					tests+=("$previous_project/$arg")
					;;
				"compiler")
					compiler="$arg"
					;;
				"config")
					config="$arg"
					;;
				"target")
					target="$arg"
					;;
				*)
					echo "invalid mode: $mode" >&2
					exit 1
					;;
				esac
			;;
	esac
done

if ! contains_element "$language" "${valid_languages[@]}"; then
	echo "invalid language: $language" >&2
	exit 1
fi

if ! contains_element "$compiler" "${valid_compilers[@]}"; then
	echo "invalid compiler: $compiler" >&2
	exit 1
fi

if ! contains_element "$config" "${valid_configs[@]}"; then
	echo "invalid config: $config" >&2
	exit 1
fi

if ! contains_element "$target" "${valid_targets[@]}"; then
	echo "invalid target: $target" >&2
	exit 1
fi

for proj in "${projects[@]}"; do
	if ! contains_element "$proj" "${existing_projects[@]}"; then
		echo "invalid project: $proj" >&2
		exit 1
	fi
done

for tes in "${tests[@]}"; do
	if ! contains_element "$tes" "${existing_tests[@]}"; then
		echo "invalid test: $tes" >&2
		exit 1
	fi
done

cmake_project_list=()
if [[ "$is_all_project" == "true" ]]; then
cmake_project_list=$(IFS=';'; printf "%s;" "${existing_projects[@]}")
cmake_project_list="${cmake_project_list%;}"  # remove trailing ;
else
cmake_project_list=$(IFS=';'; printf "%s;" "${projects[@]}")
cmake_project_list="${cmake_project_list%;}"  # remove trailing ;
fi

cmake_test_list=$(IFS=';'; echo "${tests[*]}")

mkdir -p "./_builds/$config"
cmake -S "./" -B "./_builds/$config/" \
	-Dpassed_projects="$cmake_project_list" \
	-Dpassed_tests="$cmake_test_list" \
	-DCMAKE_CXX_COMPILER="$compiler" \
	-DCMAKE_BUILD_TYPE="$config" \
	-DCMAKE_SYSTEM_NAME="$target" \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=1

if [[ ! -e './compile_commands.json' ]]; then
	ln -s _builds/Debug/compile_commands.json .
fi

if [[ "$is_build" == "true" ]]; then
	cmake --build "./_builds/$config/"
fi

