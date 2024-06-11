import os
import fileinput

def append_newline_to_files(file_extensions):
    current_directory = os.getcwd()
    for filename in os.listdir(current_directory):
        if filename.endswith(file_extensions):
            filepath = os.path.join(current_directory, filename)
            with fileinput.input(files=(filepath), inplace=True) as file:
                for line in file:
                    print(line.rstrip())
                print()  # Append a newline at the end of the file

if __name__ == "__main__":
    file_extensions = (".cc", ".h")
    append_newline_to_files(file_extensions)
    