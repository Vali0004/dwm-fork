{
  description = "NixOS in MicroVMs with X11 and DWM";

  nixConfig = {
    extra-substituters = [ "https://microvm.cachix.org" ];
    extra-trusted-public-keys = [ "microvm.cachix.org-1:oXnBc6hRE3eX5rSYdRyMYXnfzcCxC7yKPTbZXALsqys=" ];
  };

  inputs.microvm = {
    url = "github:microvm-nix/microvm.nix";
    inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, nixpkgs, microvm }: let
    system = "x86_64-linux";
  in {
    packages.${system} = {
      default = self.packages.${system}.dwm-vm;
      dwm-vm = self.nixosConfigurations.dwm-vm.config.microvm.declaredRunner;
    };
    nixosConfigurations = {
      dwm-vm = nixpkgs.lib.nixosSystem {
        inherit system;
        modules = [
          microvm.nixosModules.microvm
          ({ config, lib, pkgs, ... }: {
            imports = [
              ./config.nix
            ];
            microvm = {
              graphics.enable = true;
              hypervisor = "qemu";
              qemu.extraArgs = [
                "-device" "e1000,netdev=net0"
                "-netdev" "user,id=net0,hostfwd=tcp::5555-:22"
                "-device" "virtio-keyboard"
                "-usb"
                "-device" "usb-tablet,bus=usb-bus.0"
              ];
              shares = [{
                proto = "9p";
                tag = "ro-store";
                source = "/nix/store";
                mountPoint = "/nix/.ro-store";
              }];
              socket = "control.socket";
              volumes = [{
                mountPoint = "/var";
                image = "var.img";
                size = 256;
              }];
            };
          })
        ];
      };
    };
  };
}
