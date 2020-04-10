{ stdenv, fetchFromGitHub, gcc, cmake }:

stdenv.mkDerivation rec {
  name    = "rmath";
  version = "1.0.0";

  src = fetchFromGitHub {
    owner  = "statslabs";
    repo   = "rmath";
    rev    = "d816ef8d1719810288afd848e18b2a8d00c2ccd2";
    sha256 = "0hyrkj8pj1arjfkasfcfdaz9ifqfzckz16g9lrcbv2i8g0igybk4";
  };

  buildInputs = [
    gcc cmake
  ];
}
