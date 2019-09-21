with import <nixpkgs> {};

let common = [
    stdenv
    cmake

    sqlite

    python3
    python3Packages.pytest
]; 
in {
	test = stdenv.mkDerivation rec {
		name = "hm-env";

		buildInputs = common;
	};
}


