cd $SPIKE
sudo make -j$(nproc) -B CC=/usr/bin/gcc CXX=/usr/bin/g++
sudo make install
cd $COMP