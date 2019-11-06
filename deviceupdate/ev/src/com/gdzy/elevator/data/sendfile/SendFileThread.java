package com.gdzy.elevator.data.sendfile;

import java.io.*;
import java.net.Socket;

import static java.lang.Thread.*;

/**
 * 交互式发送文件给客户端
 */
public class SendFileThread implements Runnable {

    //和本线程相关的socket
    private Socket socket;
    //线程执行的操作，响应客户端的请求
    private DataOutputStream dataOutputStream;
    private DataInputStream dataInputStream;
    private String fileName = "C:\\Users\\Administrator\\Desktop\\BIN\\test_led.bin";
    //    String fileName = "D:\\temp\\20191011-153524.txt";
    private RandomAccessFile ra;
    //数据长度
    private int dataLen = 2048;

    private int lastLen = dataLen;

    private long sleepTime = 5000;//ms
    //
    private boolean end_flag = false;

    public SendFileThread(Socket socket) {
        this.socket = socket;
        try {
            ra = new RandomAccessFile(fileName, "r");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {

        try {
            //获取输出流，响应客户端的请求
            dataOutputStream = new DataOutputStream(socket.getOutputStream());
            //获取输入流，并读取客户端信息
            dataInputStream = new DataInputStream(socket.getInputStream());
            File file = new File(fileName);
            StringBuilder sbf = new StringBuilder();
            int r;
            String hex;
            int count = 0;
            while ((r = dataInputStream.read()) != -1  ) {
                hex = Integer.toHexString(r & 0xFF);
                if (hex.length() < 2) {
                    hex = "0" + hex;
                }
                sbf.append(hex);
                //读取到指令
                // 0x12 0x12 0x00	0x24
                // 开始发送数据
                if ("1212".equals(sbf.toString())) {
                    sbf.delete(0, sbf.length());//clean string buffer
                    int num = dataInputStream.read();
                    System.out.println("read input num " + num);
                    if (num == 0) {
                        sleep(sleepTime);
                        //first
                        int re = sendData(file, dataOutputStream, count);
                        System.out.println("第 " + (count + 1) + " 次发送，datalen=" + re);
                        if (re != dataLen) {
                             end_flag = true;
                        }
                        count++;
                    } else {
                        if(end_flag){
                            sendEndCmd();
                            break;
                        }
                        sleep(sleepTime);
                        int re = sendData(file, dataOutputStream, count);
                        System.out.println("第 " + (count + 1) + " 次发送，datalen=" + re);
                        if (re != dataLen) {
                            end_flag = true;
                        }
                        count++;
                    }
                    dataInputStream.read();

                }

            }


        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        } finally {
            try {
                releaseResource();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void releaseResource() throws IOException {
        if (dataOutputStream != null)
            dataOutputStream.close();
        if (dataInputStream != null)
            dataInputStream.close();
        if (socket != null)
            socket.close();
        if (ra != null)
            ra.close();

        System.out.println("end data handler...");
    }

    private void sendEndCmd() throws InterruptedException, IOException {
        System.out.println("end of file,send cmd xxxx");
        byte[] endCmd = new byte[]{
                Byte.parseByte(String.valueOf(0x11)),
                Byte.parseByte(String.valueOf(0x11)),
                Byte.parseByte(String.valueOf(0x11)),
                Byte.parseByte(String.valueOf(0x33))
        };
        sleep(sleepTime);
        dataOutputStream.write(endCmd);
        dataOutputStream.flush();
        sleep(sleepTime);

        App.printByteArrayToHex(endCmd);
        releaseResource();
    }

    private int sendData(File file, DataOutputStream dataOutputStream, int count) throws IOException {
        //头帧			 长度		 地址			   序号							 数据				  校验
        //俩个字节		 俩个字节	 四个字节（累计加）   一个字节						  512字节			一个字节累计加
        //0x11 0x11 	 512      	  0x8004000			0x01（累计加，第一次为0x01）	  512字节			累计加
        if (file.exists()) {
            long fileLen = file.length();
            //发送的数据报文长度
            int len = 2 + 2 + 4 + 1 + dataLen + 1;
            byte[] bytes = new byte[len];
            headers(count, bytes, dataLen);
            //数据
            byte[] dataBytes = new byte[dataLen];
//            FileInputStream fileInputStream = new FileInputStream(file);
//            fileInputStream.skip(dataLen);
//            int realDataLen = fileInputStream.read(dataBytes,0,dataLen);
//            System.out.println(ra.getFilePointer());
            //ra.skipBytes(count*dataLen);
            //System.out.println(ra.getFilePointer());
            int realDataLen = ra.read(dataBytes, 0, dataLen);
            if (realDataLen != -1) {
                if (realDataLen == dataLen) {
                    System.arraycopy(dataBytes, 0, bytes, 9, dataLen);
                } else {
                    //最后一包 长度不够dataLen
                    len = 2 + 2 + 4 + 1 + realDataLen + 1;
                    bytes = new byte[len];
                    headers(count, bytes, realDataLen);
                    System.arraycopy(dataBytes, 0, bytes, 9, realDataLen);
                }
            } else {
                System.out.println("file length not enough!");
                return -1;
            }
            //校验 暂定 0x01
            int validate = 0x01;
            bytes[bytes.length - 1] = Byte.parseByte(String.valueOf(validate));
            dataOutputStream.write(bytes, 0, len);
            dataOutputStream.flush();
            App.printByteArrayToHex(bytes);
            return realDataLen;
        } else {
            System.out.println("file not found!");
            return -1;
        }
    }

    private void headers(int count, byte[] bytes, int dataLen) {
        //头帧
        int head = 0x11;
        bytes[0] = Byte.parseByte(String.valueOf(head));
        bytes[1] = Byte.parseByte(String.valueOf(head));
        //长度
        byte[] dataLenBytes = App.intTo2ByteArray(dataLen);
        System.arraycopy(dataLenBytes, 0, bytes, 2, 2);
        //  地址 四个字节（累计加）
        int address = 0x08005000;
        address += count * this.dataLen ;
        byte[] addressBytes = App.intTo4ByteArray(address);
        System.arraycopy(addressBytes, 0, bytes, 4, 4);
        // 序号
        int seq = 0x01;
        seq += count;
        bytes[8] = Byte.valueOf(String.valueOf(seq));
    }
}
