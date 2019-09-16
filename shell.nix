with import <nixpkgs> {};

{
	shell = stdenv.mkDerivation rec {
		name = "hm-env";

		buildInputs = [
			stdenv
			cmake

			sqlite
			kdevelop

			python3
			python3Packages.pytest
		];

		shellHook = ''
			# export something
			#nix-store --add-root kdev --indirect -r ${kdevelop}
		'';
	};
}


