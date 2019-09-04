with import <nixpkgs> {};

{
	shell = stdenv.mkDerivation rec {
		name = "hm-env";

		buildInputs = [
			stdenv
			cmake

			sqlite
		];

		shellHook = ''
			# export something
		'';
	};
}


