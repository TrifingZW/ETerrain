import argparse
import numpy as np


def generate_font_array(font_path, output_path):
    # 读取二进制文件
    binary_data = np.fromfile(font_path, dtype=np.uint8)

    # 写入头文件
    with open(output_path, "w") as output_file:
        output_file.write("static unsigned char font_ttf[] = {\n")
        output_file.write(", ".join(f"0x{byte:02x}" for byte in binary_data))
        output_file.write("\n};\n")
        output_file.write(f"static unsigned int font_ttf_len = {len(binary_data)};\n")


def main():
    # 设置命令行参数解析
    parser = argparse.ArgumentParser(description="将字体文件转换为 C 头文件")
    parser.add_argument("font_path", help="字体文件路径")
    parser.add_argument("output_path", help="输出的头文件路径")

    # 解析命令行参数
    args = parser.parse_args()

    # 调用函数生成头文件
    generate_font_array(args.font_path, args.output_path)
    print(f"头文件已生成: {args.output_path}")


if __name__ == "__main__":
    main()
