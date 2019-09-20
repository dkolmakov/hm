with import <nixpkgs> {};

let common = [
    stdenv
    cmake

    sqlite

    python3
    python3Packages.pytest
]; 
in {
	dev = stdenv.mkDerivation rec {
		name = "hm-env";

		buildInputs = common ++ [
			kdevelop
		];

		shellHook = ''
			# export something
			#nix-store --add-root kdev --indirect -r ${kdevelop}
		'';
	};

	test = stdenv.mkDerivation rec {
		name = "hm-env";

		buildInputs = common;

		shellHook = ''
			# export something
			#nix-store --add-root kdev --indirect -r ${kdevelop}
		'';
	};
}


