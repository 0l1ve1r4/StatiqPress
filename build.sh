#!/bin/bash

APPLICATION_NAME="StatiqPress"
APPLICATION_DIR="./$APPLICATION_NAME"

# Function to build the binary
build_binary() {
    cd src || { echo "Failed to change directory to src"; exit 1; }
    make clean || { echo "Failed to clean build"; exit 1; }
    make || { echo "Failed to build the binary"; exit 1; }
    cd - || { echo "Failed to return to the previous directory"; exit 1; }
}

# Function to create application directory structure
create_application_dir() {
    # Remove old application if it exists
    if [ -d "$APPLICATION_DIR" ]; then
        rm -rf "$APPLICATION_DIR" || { echo "Failed to delete old application directory"; exit 1; }
    fi

    # Create new application directory structure
    mkdir -p "$APPLICATION_DIR"/{config,bin,lib,include,uploads} || { echo "Failed to create application directories"; exit 1; }

    # Move built files to the new application directory
    cp "src/StatiqPress" "$APPLICATION_DIR/" || { echo "Failed to move SqPress to application directory"; exit 1; }
    cp "src/icon.ico" "$APPLICATION_DIR/" || { echo "Failed to move StatiqPress.ico to application directory"; exit 1; }

    # Create the default configuration file
    create_default_config
}

# Function to create a default configuration file
create_default_config() {
    cat <<EOF > "$APPLICATION_DIR/config/config.ini"
[General]
title = StatiqPress
screen_width = 1600
screen_height = 960

[Github]
repository_url = www.github.com/username/repository
uploads_path = static/img/
EOF
}

# Main execution flow
build_binary
create_application_dir
cd src/ && make clean || { echo "Failed to clean up object files"; exit 1; }

# read -p "Do you want to run the application now? (y/n): " run_app
run_app="y"
if [ "$run_app" == "y" ]; then
    cd "../$APPLICATION_DIR" || { echo "Failed to change directory to application directory"; exit 1; }
    ./StatiqPress
fi

exit 0
