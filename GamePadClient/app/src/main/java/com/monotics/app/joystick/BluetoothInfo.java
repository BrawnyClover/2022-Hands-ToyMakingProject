package com.monotics.app.joystick;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import java.io.OutputStream;

public class BluetoothInfo
{
    private BluetoothDevice bluetoothDevice; // 블루투스 디바이스
    private BluetoothSocket bluetoothSocket = null; // 블루투스 소켓
    private OutputStream outputStream = null; // 블루투스에 데이터를 출력하기 위한 출력 스트림

    public BluetoothInfo(){}
    public BluetoothInfo(BluetoothDevice bluetoothDevice, BluetoothSocket bluetoothSocket, OutputStream outputStream){
        this.bluetoothDevice = bluetoothDevice;
        this.bluetoothSocket = bluetoothSocket;
        this.outputStream = outputStream;
    }
}
