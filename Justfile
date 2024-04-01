arch             := `uname -m`
build_dir        := "build"
debug_dir        := build_dir / "debug"
debug_target     := debug_dir / "bin" / "flog"
release_dir      := build_dir / "release"
release_target   := release_dir / "bin" / "flog"
man_dir          := "man"
man_source       := man_dir / "flog.1.md"
man_target       := man_dir / "flog.1"
sarif_file       := "codeql-analysis.sarif"
codeql_dir       := "codeql"
codeql_build_dir := codeql_dir / "build"
codeql_db_dir    := codeql_dir / "db"

# generate build artifacts and run unit tests
@all: build test

# generate debug build
@build:
    #!/usr/bin/env bash
    set -euo pipefail
    if [[ ! -d "{{debug_dir}}" ]]; then
        cmake -S . -B "{{debug_dir}}" -DCMAKE_BUILD_TYPE=Debug -DUNIT_TESTING=on
    fi
    cmake --build "{{debug_dir}}"

# generate release build
@build-release:
    #!/usr/bin/env bash
    set -euo pipefail
    if [[ ! -d "{{release_dir}}" ]]; then
        cmake -S . -B "{{release_dir}}" -DCMAKE_BUILD_TYPE=Release -DUNIT_TESTING=on
    fi
    cmake --build "{{release_dir}}"

# run debug build unit tests
@test: build
    ctest -V --test-dir "{{debug_dir}}/test"

# run release build unit tests
@test-release: build-release
    ctest -V --test-dir "{{release_dir}}/test"

# remove build directories and artifacts
@clean:
    rm -rf \
        "{{codeql_dir}}" \
        "{{build_dir}}" \
        "{{man_target}}" \
        "{{sarif_file}}" \
        *.xz

# build the man page
@man:
    pandoc --standalone -f markdown -t man "{{man_source}}" > "{{man_target}}"

# build and show the man page
@show-man: man
    man "{{man_target}}"

# perform static code analysis
@analyse:
    #!/usr/bin/env bash
    set -euo pipefail

    [[ -d "{{codeql_build_dir}}" ]] && rm -rf "{{codeql_build_dir}}"
    [[ -d "{{codeql_db_dir}}" ]] && rm -rf "{{codeql_db_dir}}"
    [[ -f "{{sarif_file}}" ]] && rm -f "{{sarif_file}}"

    mkdir -p "{{codeql_build_dir}}"

    codeql database create "{{codeql_db_dir}}" \
        --source-root="./src" \
        --language="cpp" \
        --command="cmake -S .. -B ../{{codeql_build_dir}}" \
        --command="cmake --build ../{{codeql_build_dir}}"

    codeql database analyze "{{codeql_db_dir}}" \
        --format="sarif-latest" \
        --output="{{sarif_file}}" \
        "codeql/cpp-queries:codeql-suites/cpp-security-and-quality.qls"

# generate release package
@package version: build-release test-release man
    #!/usr/bin/env bash
    set -euo pipefail
    echo "Packaging with version number: {{version}} for architecture: {{arch}}"

    tmp_dir=$(mktemp -d)
    tar_dir="flog-{{version}}"
    tar_file="flog-{{version}}-darwin-{{arch}}.tar.xz"

    mkdir -p "${tmp_dir}/${tar_dir}/bin"
    mkdir -p "${tmp_dir}/${tar_dir}/usr/share/man/man1"

    cp "{{release_target}}" "${tmp_dir}/${tar_dir}/bin/"
    cp "{{man_target}}" "${tmp_dir}/${tar_dir}/usr/share/man/man1/"

    tar -C "${tmp_dir}" -cvJf "${tar_file}" "${tar_dir}"

    echo "Generated release package: ${tar_file}"
