with import <nixpkgs> {};

{
	shell = stdenv.mkDerivation rec {
		name = "hm-env";

		buildInputs = [
			stdenv
			cmake

			sqlite
			#kdevelop
		];

		shellHook = ''
			# export something
			#nix-store --add-root kdev --indirect -r ${kdevelop}
		'';
	};
}


