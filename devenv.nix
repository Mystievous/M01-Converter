{
  pkgs,
  inputs,
  ...
}:
let

  DEVKITPRO = "${pkgs.devkitNix.devkitARM}/opt/devkitpro";
  DEVKITARM = "${DEVKITPRO}/devkitARM";
in
{
  name = "M01-Converter";

  overlays = [
    inputs.devkitNix.overlays.default
  ];

  packages = [
    pkgs.devkitNix.devkitARM
  ];

  env = {
    inherit DEVKITPRO DEVKITARM;
  };

  languages.cplusplus.enable = true;

  enterShell = ''
    echo "Nintendo DS (devkitARM) shell ready."
    command -v arm-none-eabi-gcc >/dev/null && arm-none-eabi-gcc --version | head -n1
    echo "DEVKITPRO=$DEVKITPRO"
    echo "DEVKITARM=$DEVKITARM"
  '';

  # See full reference at https://devenv.sh/reference/options/
}
