{ config, lib, pkgs, ... }:

{
  boot = {
    extraModulePackages = [ ];
    initrd = {
      availableKernelModules = [
        "usb_storage"
        "usbhid"
      ];
      kernelModules = [ ];
    };
    kernelModules = [
      "drm" "virtio_gpu"
    ];
  };

  console.useXkbConfig = true;

  environment = {
    sessionVariables = {
      DISPLAY = ":0";
      XDG_SESSION_TYPE = "x11";
    };
    shellAliases = {
      l = null;
      ll = null;
      lss = "ls --color -lha";
    };
    systemPackages = with pkgs; [
      alsa-utils
      bridge-utils
      curl
      fastfetch
      flameshot
      google-chrome
      firefox
      kodiPackages.inputstream-adaptive
      vulkan-extension-layer
      vulkan-tools
      vulkan-validation-layers
      xdg-launch
      xdg-utils
      xhost
      zip
    ];
    variables.CM_LAUNCHER = "rofi";
  };

  fonts.packages = [ pkgs.nerd-fonts.dejavu-sans-mono ];

  hardware.graphics = {
    enable = true;
    enable32Bit = true;
  };

  i18n = {
    defaultLocale = "en_US.UTF-8";
    supportedLocales = [ "en_US.UTF-8/UTF-8" ];
  };

  networking = {
    firewall = {
      allowedTCPPorts = [ 22 8080 ];
      allowedUDPPorts = [ 8080 ];
    };
    hostName = "nixos-vm";
    useDHCP = true;
  };

  nix.settings = {
    experimental-features = [
      "nix-command"
      "flakes"
    ];
    substituters = [
      "https://hydra.fuckk.lol"
      "https://cache.nixos.org/"
    ];
    trusted-users = [
      "root"
      "vali"
      "@wheel"
    ];
    trusted-public-keys = [
      "hydra.fuckk.lol:6+mPv9GwAFx/9J+mIL0I41pU8k4HX0KiGi1LUHJf7LY="
      "cache.nixos.org-1:6NCHdD59X431o0gWypbMrAURkbJ16ZPMQFGspcDShjY="
    ];
  };

  nixpkgs = {
    config.allowUnfree = true;
    hostPlatform = "x86_64-linux";
  };

  programs = {
    git.enable = true;
  };

  qt = {
    enable = true;
    platformTheme = "gnome";
    style = "adwaita-dark";
  };

  security = {
    rtkit.enable = true;
    sudo = {
      enable = true;
      wheelNeedsPassword = true;
    };
  };

  services = {
    getty.autologinUser = "vali";
    openssh = {
      enable = true;
      settings.PasswordAuthentication = false;
    };
    displayManager.autoLogin.user = "kodi";
    xserver = {
      enable = true;
      desktopManager = {
        kodi = {
          enable = true;
          package = (pkgs.kodi.withPackages (kodiPkgs: with kodiPkgs; [
            jellyfin
            inputstream-adaptive
          ]));
        };
      };
      displayManager = {
        startx = {
          enable = false;
          #generateScript = true;
        };
        lightdm.greeter.enable = false;
      };
      logFile = "/var/log/Xorg.0.log";
      xkb.layout = "us";
    };
  };

  system = {
    activationScripts.fixXinitrc.text = ''
      # Copy the original script, and set as writable
      #install -Dm755 /etc/X11/xinit/xinitrc /root/.xinitrc
      #install -Dm755 /etc/X11/xinit/xinitrc /home/vali/.xinitrc
      # Copy to both home directories
      #chown root:root root/.xinitrc
      #chown vali:users /home/vali/.xinitrc
    '';
    stateVersion = "25.11";
  };

  systemd.watchdog.rebootTime = "0";

  time.timeZone = "America/Detroit";

  users = let
    ssh_keys = [
      "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDJR3qDc8r2kbg6Q+A0dk7E6fC/wdlySBKb8X+8XgRGJg6huXaCTPZbAyvzt1IvxY69IdBymExjUie7YuFOLOKi5wisfw6d1yVjrhaoZWvXTz6eyF0ssAzM1BbqJsHU2dahQnNo7ThUguR365woBaw1UrZHEjlAiX16NxDVEyaXNImDjlQKBiAyDaa/pOCe1GUYwPgXHJMwF+6JbY+pGYAm6AvvsnjhLO0kyzwv1hSOd4qlzSobkDE9FQMbJD7uV+D1cXAv2ERdf/h9/L5dUcOEUscES+wg8ezLOhaBmq8TT9K3gmhMa47zNQU1WUAg39n+2+/Dwix0j7GNsNZdbp6B vali@nixos-amd"
    ];
  in {
    extraUsers.kodi.isNormalUser = true;
    groups.users = {};
    users = {
      root.openssh.authorizedKeys.keys = ssh_keys;
      vali = {
        extraGroups = [
          "render"
          "tty"
          "wheel"
          "video"
        ];
        group = "users";
        isNormalUser = true;
        password = "773415";
        openssh.authorizedKeys.keys = ssh_keys;
      };
    };
  };

  xdg.portal = {
    config.common.default = [ "gtk" ];
    enable = true;
    extraPortals = with pkgs; [ xdg-desktop-portal xdg-desktop-portal-gtk ];
    xdgOpenUsePortal = true;
  };
}
