with import <nixpkgs> {};

let deps = {
    cram_ = pkgs.python3Packages.buildPythonPackage rec {
        name = "cram";
        src = /home/dmitry/projects/cram;
        doCheck = false;
    };

    common = [
        stdenv
        cmake

        python3
        python3Packages.pytest
    ]; 
};
in with deps; {
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
	};
}


