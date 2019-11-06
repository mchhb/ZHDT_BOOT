package com.gdzy.elevator.data.sendfile;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.Socket;

public class ClientTest extends Socket {

    private static final String SERVER_IP = "127.0.0.1"; // 服务端IP
    private static final int SERVER_PORT = 8900; // 服务端端口

    private Socket client;

    private FileInputStream fis;
    private DataInputStream din;
    private DataOutputStream dos;
    public ClientTest() throws Exception {
        super(SERVER_IP, SERVER_PORT);
        this.client = this;
        System.out.println("Cliect[port:" + client.getLocalPort() + "] 成功连接服务端");
    }
    public static void main(String[] args) {
        try {
            ClientTest client = new ClientTest(); // 启动客户端连接
            client.readFile(); // 传输文件

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void readFile() throws IOException {
        din = new DataInputStream( client.getInputStream());
        dos = new DataOutputStream( client.getOutputStream());
        int head1 = 0x12;
        int head2 = 0x24;
        byte[] b = new byte[] {
                Byte.valueOf(  String.valueOf(head1) ),
                Byte.valueOf(String.valueOf(head1) ),
                Byte.valueOf( "0" ),
                Byte.valueOf(String.valueOf(head2) )
         };
        dos.write( b );
        dos.flush();
        int r ;
        StringBuffer bf = new StringBuffer();
        String hex;
        int count = 0 ;
        while((r=din.read())!=-1){
            hex = Integer.toHexString(r & 0xFF);
            if (hex.length() < 2) {
                hex = "0" + hex;
            }
            bf.append( hex );
            if(bf.length() ==522){
                System.out.println(bf.toString());
                bf.delete(0,522);
                count++;
                b = new byte[] {
                        Byte.valueOf(  String.valueOf(head1) ),
                        Byte.valueOf(String.valueOf(head1) ),
                        Byte.valueOf( String.valueOf(count) ),
                        Byte.valueOf(String.valueOf(head2) )
                };
                dos.write( b );
                dos.flush();
            }
        }

    }

}
