with import <nixpkgs> {};

stdenv.mkDerivation rec {
    version = "0.0.1";
    name = "hm-${version}";
    path = dirOf "./build/";
    
    src = fetchurl {
            url = "https://github.com/dkolmakov/hm/archive/v${version}.tar.gz";
            sha256 = "06jjqxf1s8pv9aygc5s43jqm8gib5x4w8wgh1lpdrmjbinc38sip";
          };

    buildInputs = [ stdenv cmake ];
    
    meta = {
        inherit version;
        description = ''Command line history manager for Bash'';
        license = stdenv.lib.licenses.asl20;
    };
}


