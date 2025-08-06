{
  inputs = {
    utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:nixos/nixpkgs";
  };
  outputs = { self, utils, nixpkgs }:
  (utils.lib.eachSystem [ "x86_64-linux" ] (system:
  let
    pkgsLut = {
      x86_64-linux = nixpkgs.legacyPackages.${system}.extend self.overlay;
    };
    pkgs = pkgsLut.${system};
  in {
    packages = {
      inherit (pkgs) default dwm dwm-static;
    };
    devShell = pkgs.dwm;
  })) // {
    overlay = self: super:
    let
      staticOverlay = self: super: {
        waylandSupport = false;
        libayatana-appindicator = null;
        libdecorSupport = false;
        libudevSupport = false;
        ibusSupport = false;
        pipewireSupport = false;
        pulseaudioSupport = false;
      };
    in {
      dwm-static = (self.extend staticOverlay).pkgsStatic.dwm;
      default = self.callPackage ./dwm.nix {};
      dwm = self.callPackage ./dwm.nix {};
    };
  };
}