{
  pkgs,
  ...
}:

{
  name = "M01-Converter";

  packages = with pkgs; [
    just
  ];

  # https://devenv.sh/languages/
  languages.cplusplus.enable = true;

  # See full reference at https://devenv.sh/reference/options/
}
