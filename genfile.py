import shutil
import os

def copy_lua_to_src():
    source_dir = 'lua'
    target_dir = 'src'

    # Check if the source directory exists to avoid errors
    if not os.path.exists(source_dir):
        print(f"Error: The source directory '{source_dir}' does not exist.")
        return

    # Create the target directory if it doesn't already exist
    if not os.path.exists(target_dir):
        os.makedirs(target_dir)
        print(f"Created target directory: {target_dir}")

    # Iterate through all files in the source directory
    for filename in os.listdir(source_dir):
        source_path = os.path.join(source_dir, filename)
        target_path = os.path.join(target_dir, filename)

        # Ensure we are only copying files (not subfolders)
        if os.path.isfile(source_path):
            shutil.copy2(source_path, target_path)
            print(f"Copied: {filename}")

    print("\nFile transfer complete.")

if __name__ == "__main__":
    copy_lua_to_src()