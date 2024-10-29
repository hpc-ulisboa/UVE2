cd $SPIKE
sudo make -j$(nproc) -B
sudo make install
cd $COMP