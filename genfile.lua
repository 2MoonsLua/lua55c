local source_dir = "lua"
local target_dir = "src"

-- 1. Create the target directory if it doesn't exist
-- 'mkdir -p' works on Unix; 'if not exist' syntax is for Windows
local function create_dir(path)
    if package.config:sub(1,1) == "\\" then -- Windows check
        os.execute("if not exist " .. path .. " mkdir " .. path)
    else
        os.execute("mkdir -p " .. path)
    end
end

-- 2. Copy the files
local function copy_files(src, dest)
    create_dir(dest)
    
    local cmd
    if package.config:sub(1,1) == "\\" then
        -- Windows command (xcopy /y copies files, /e would do folders)
        cmd = string.format("xcopy %s\\* %s\\ /y /i", src, dest)
    else
        -- Unix/Linux/macOS command
        cmd = string.format("cp -r %s/* %s/", src, dest)
    end
    
    local success = os.execute(cmd)
    if success then
        print("Successfully copied files from " .. src .. " to " .. dest)
    else
        print("Error: Failed to copy files.")
    end
end

copy_files(source_dir, target_dir)