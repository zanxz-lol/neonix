#!/usr/bin/env python3

# kconfiglib is mean to us
import sys

def show_usage(prog_name):
    print(("{}: [config_file] [header_file_path]").format(prog_name))

def generate_header_file(path, output) -> int:
    config_lines = []
    try:
        with open(path, 'r') as file:
            config_lines = file.readlines()
    except Exception as error:
        print(("Could not generate header file: {}").format(error.args))
        return 1

    header_lines = []
    header_lines.append("#ifndef __NAHO_CONFIG_H")
    header_lines.append("#define __NAHO_CONFIG_H")
    header_lines.append("")
    for line in config_lines:
        line = line.strip()
        if line.startswith("#") or line == "":
            continue
        elif "CONFIG_" in line:
            option, value = line.split("=")
            option = option.strip()
            value = value.strip()
            if value.isdigit():  # Check if the value is a number
                header_lines.append("#define " + option + " " + value)
            else:
                if len(value) < 2:
                    if value.startswith("y"):
                        header_lines.append("#define " + option + " 1")
                else:
                    header_lines.append("#define " + option + " \"" + value + "\"")
        else:
            header_lines.append(line)
    header_lines.append("")
    header_lines.append("#endif")

    with open(output, 'w') as f:
        f.write("\n".join(header_lines))
    return 0

def main(argc, argv):
    if argc < 2:
        show_usage(argv[0])
        return 1
    return generate_header_file(argv[1], argv[2])

if __name__ == "__main__":
    exit(main(len(sys.argv), sys.argv))
