set -euxo pipefail

apt update -y

apt install gcc-11-i686-linux-gnu -y
apt install gcc-arm-none-eabi -y
apt install gcc-11-riscv64-linux-gnu -y
apt install g++-11-i686-linux-gnu -y
apt install g++-11-riscv64-linux-gnu -y

apt install python3 -y

pip install crc