{ stdenv, callPackage, gcc, cmake, gnumake, fetchFromGitHub, valgrind, kcachegrind }:

let
  shortName = "stat-r-in-c";
  version   = "0.0.1";

  rmath = callPackage ./rmath.nix {
    inherit stdenv;
    inherit gcc;
    inherit cmake;
    inherit fetchFromGitHub;
  };
in
  stdenv.mkDerivation rec {
    name = "${shortName}-${version}";
    src  = ./.;

    buildInputs = [
      gcc
      cmake
      rmath
      valgrind
      kcachegrind
    ];
  }
