#!/bin/zsh

echo "[\033[92m+\033[0m] \033[92mInstalling\033[0m Rishka CC..."

echo "[\033[92m+\033[0m] Checking up Rust compiler and git..."
sudo apt-get -y install rust > /dev/null 2>&1
sudo apt-get -y install git > /dev/null 2>&1

echo "[\033[92m+\033[0m] Cloning source files from GitHub..."
git clone --quiet --depth 1 https://github.com/nthnn/rishka.git

echo "[\033[92m+\033[0m] Building Rishka CC from sources..."
cd rishka/tools/rishka-cc && cargo build --release --quiet
echo "[\033[92m+\033[0m] Built \033[36msuccessfully\033[0m!"

if [[ -e "/usr/local/bin/rishka-cc" ]]; then
    echo "[\033[91m-\033[0m] Removing previously installed Rishka CC..."
    sudo rm -rf /usr/local/bin/rishka-cc
fi

echo "[\033[92m+\033[0m] Installing Rishka CC to default bin path..."
sudo mv target/release/rishka-cc /usr/local/bin/rishka-cc

echo "[\033[92m+\033[0m] Cleaning up..."
cargo clean && cd ../../..
sudo rm -rf rishka

echo "[\033[92m+\033[0m] Successfully \033[36minstalled\033[0m!"