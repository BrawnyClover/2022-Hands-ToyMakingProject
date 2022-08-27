package com.monotics.app.joystick;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import com.monotics.view.joystick.JoystickView;
import java.io.IOException;
import java.io.OutputStream;
import java.util.*;

public class MainActivity extends AppCompatActivity
{
    private int GET_BLT_DEVICE = 10;
    private static final int REQUEST_ENABLE_BT = 10; // 블루투스 활성화 상태
    private BluetoothAdapter bluetoothAdapter; // 블루투스 어댑터
    private Set<BluetoothDevice> devices; // 블루투스 디바이스 데이터 셋
    private BluetoothDevice bluetoothDevice = null;
    private BluetoothSocket bluetoothSocket = null;
    private OutputStream outputStream = null;
    private String pairingInfoText = null;
    private boolean isBTActivated = false;
    private TextView pairingInfoView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter == null) {
            //장치가 블루투스를 지원하지 않는 경우
            Log.d("Bluetooth", "not support");
        }

        else {
            // 장치가 블루투스를 지원하는 경우.
            if(bluetoothAdapter.isEnabled()) { // 블루투스가 활성화 상태 (기기에 블루투스가 켜져있음)
                Log.d("Bluetooth", "already enabled");
                selectBluetoothDevice(); // 블루투스 디바이스 선택 함수 호출
            }
            else {
                Log.d("Bluetooth", "not enabled");
                Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(intent, REQUEST_ENABLE_BT);
            }
        }
        pairingInfoView = findViewById(R.id.pairing_info);
        JoystickView joystickView = findViewById(R.id.joystick);
        TextView angleValueView = findViewById(R.id.value_angle);
        TextView strengthValueView = findViewById(R.id.value_strength);

        JoystickView joystickRightView = findViewById(R.id.joystick_right);
        TextView angleValueRightView = findViewById(R.id.value_angle_right);
        TextView strengthValueRightView = findViewById(R.id.value_strength_right);
        joystickView.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int x, int y) {
                angleValueView.setText(String.format("angle : %d", x));
                strengthValueView.setText(String.format("strength : %d", y));
                if(y == 0){
                    sendData(String.valueOf('s'));
                }
                else {
                    char order = calcOrder(x);
//                sendData(String.format("L%3d%3d", x, y));
                    sendData(String.valueOf(order));
                }
            }
        });
        joystickRightView.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int x, int y) {
                angleValueRightView.setText(String.format("angle : %d", x));
                strengthValueRightView.setText(String.format("strength : %d", y));
                sendData(String.format("a%03d%03d",x,y));
//                sendData(String.format("R%3d%3d", x, y));
            }
        });

        pairingInfoView.setText(pairingInfoText);
    }

    private char calcOrder(int x){
        if((0<=x && x<45) || (360>=x && 315<x)){
            return 'r';
        }
        else if(45<=x && x<135){
            return 'f';
        }
        else if(135<=x && x<225){
            return 'l';
        }
        else{
            return 'b';
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch(requestCode) {
            case REQUEST_ENABLE_BT:
                if (requestCode == RESULT_OK) {
                    isBTActivated = true;
                    selectBluetoothDevice();
                } else {
                    isBTActivated = false;
                }
                break;
        }
    }

    public void selectBluetoothDevice()
    {
        devices = bluetoothAdapter.getBondedDevices();
        int pairedDeviceCount = devices.size();
        if(pairedDeviceCount == 0){
            // do pairing
        }
        else{
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("Devices");
            List<String> list = new ArrayList<>();
//            for(BluetoothDevice bluetoothDevice : devices){
//
//                list.add(bluetoothDevice.getName());
//            }
            list.add("BLITZ1");
            list.add("BLITZ2");
            list.add("취소");

            final CharSequence[] charSequences = list.toArray(new CharSequence[list.size()]);
            list.toArray(new CharSequence[list.size()]);

            builder.setItems(charSequences, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialogInterface, int i) {
                    connectDevice(charSequences[i].toString());
                }
            });
            builder.setCancelable(false);
            AlertDialog alertDialog = builder.create();
            alertDialog.show();
        }
    }

    public void connectDevice(String deviceName)
    {
        if(deviceName.equals("취소")){
            isBTActivated = false;
            return;
        }
        for(BluetoothDevice tmpDevice : devices){
            if(deviceName.equals(tmpDevice.getName())) {
                bluetoothDevice = tmpDevice;
                pairingInfoText = bluetoothDevice.getName();
                pairingInfoView.setText(pairingInfoText);
                Log.d("deviceName", pairingInfoText);
                break;
            }
            Log.d("connect","found device");
        }
        UUID uuid = java.util.UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
        try {
            bluetoothSocket = bluetoothDevice.createRfcommSocketToServiceRecord(uuid);
            bluetoothSocket.connect();
            outputStream = bluetoothSocket.getOutputStream();
            sendData("Connected");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void sendData(String text)
    {
        text += "\n";
        try{
            outputStream.write(text.getBytes());
        }catch (IOException e)
        {
            e.printStackTrace();
        }
    }

    public void onArmShootBtnClick(View view) {
        try{
        sendData("H");
        }catch (Exception e){

        }

    }
}