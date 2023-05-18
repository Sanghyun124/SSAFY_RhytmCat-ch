# 리듬캣치 포팅매뉴얼

## 1️⃣ 리듬캣치

---

### 언리얼

```powershell
// 오큘러스퀘스트2 에어링크를 통해 연결

// 빌드 파일 설치 후 rhythmcatch.exe 실행

// 스마트 글러브가 없을시 플레이 방법
1 리시브, 2 배트, 3 펀치, 4 라켓, 5 번트, 6 칼, 7 스파이크

```

## 2️⃣ 스마트글러브

---

```bash
sudo apt-get update
sudo apt install git bc bison flex libssl-dev make vim gcc
sudo apt-get install build-essential
sudo apt-get install raspberrypi-kernel-headers
git clone --depth=1 --branch rpi-5.1.y https://github.com/raspberrypi/linux
cd linux
KERNEL=kernel
make bcmrpi_defconfig
make -j4 zImage modules dtbs
sudo make modules_install
sudo cp arch/arm/boot/dts/*.dtb /boot/
sudo cp arch/arm/boot/dts/overlays/*.dtb* /boot/overlays/
sudo cp arch/arm/boot/dts/overlays/README /boot/overlays/
sudo cp arch/arm/boot/zImage /boot/$KERNEL.img
```

### 2. 세팅

```bash
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi/
./build
sudo nano /boot/config.txt
cd ~
git clone https://lab.ssafy.com/s08-final/S08P31C108.git
cd S08P31C108/smartglove
make all
sudo mv smartglove.ko /lib/modules/$(uname -r)/
sudo depmod
sudo nano /etc/modules  --> 가장 밑 줄에 smartglove적고 저장
sudo reboot
```

### 3. 사용자코드 자동 로딩

```powershell
sudo crontab -e 
--> 가장 밑 줄에 @reboot /home/pi/S08P31C108/smartglove_read &를 적고 저장

nano /home/pi/.bashrc
--> 가장 밑 줄에 /home/pi/S08P31C108/smartglove_read를 적고 저장(터미널에 찍고싶으면)

sudo reboot
```