{
  pkgs ? import <nixpkgs> {},
  fetchFromGitHub
}:

pkgs.stdenv.mkDerivation rec {
  name = "minishell";
  version = "1.11";

  src = fetchFromGitHub {
    repo = "minishell";
    owner = "Kek5chen";
    rev = "master";
    hash = "sha256-sUypVOFE306G3b2fzEO7mvm9KOKduVu/Xn/5cjLqtFU=";
  };

  buildInputs = with pkgs; [ gcc readline ];

  makeFlags = [ "PREFIX=$(out)" ];

  enableParallelBuilding = true;

  meta = {
    description = "A mini shell";
    homepage = "https://github.com/Kek5chen/minishell";
  };
}

