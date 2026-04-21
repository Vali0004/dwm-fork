{ lib
, stdenv
, fetchzip
, libx11
, libxdamage
, libxcomposite
, libxft
, libxinerama
, writeText
, yajl }:

stdenv.mkDerivation (finalAttrs: {
  pname = "dwm";
  version = "6.5";

  src = ./.;

  buildInputs = [
    libx11
    libxcomposite
    libxdamage
    libxft
    libxinerama
    yajl
  ];

  preBuild = ''
    makeFlagsArray+=(
      "PREFIX=$out"
      "CC=$CC"
      ${lib.optionalString stdenv.hostPlatform.isStatic ''
        LDFLAGS="$(${stdenv.cc.targetPrefix}pkg-config --static --libs x11 xinerama xft)"
      ''}
    )
  '';

  meta = {
    homepage = "https://dwm.suckless.org/";
    description = "Extremely fast, small, and dynamic window manager for X";
    longDescription = ''
      dwm is a dynamic window manager for X. It manages windows in tiled,
      monocle and floating layouts. All of the layouts can be applied
      dynamically, optimising the environment for the application in use and the
      task performed.
      Windows are grouped by tags. Each window can be tagged with one or
      multiple tags. Selecting certain tags displays all windows with these
      tags.
    '';
    license = lib.licenses.mit;
    maintainers = with lib.maintainers; [ neonfuz ];
    platforms = lib.platforms.all;
    mainProgram = "dwm";
  };
})