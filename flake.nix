{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }@inputs: let 
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
  in {
    devShells.${system}.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        gcc
        gdb
        valgrind
        cmake
        clang-tools
        glfw
        glsl_analyzer
      ];

      C_INCLUDE_PATH = "${pkgs.glibc.dev}/include";
      LIBRARY_PATH="${pkgs.glfw}/lib:$LIBRARY_PATH";

      shellHook = ''
        fish && exit
      '';

    };
  };
}
