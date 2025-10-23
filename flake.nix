{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { nixpkgs, ... }: 
  let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages."${system}";
  in
  {
    devShells.${system}.default =
    let
      llvmPackages = pkgs.llvmPackages_latest;
    in
      pkgs.mkShell {
        stdenv = llvmPackages.stdenv;
        packages = with pkgs; [
          llvmPackages.clangUseLLVM
          cmake
          ninja
        ];
      };


  };
}
