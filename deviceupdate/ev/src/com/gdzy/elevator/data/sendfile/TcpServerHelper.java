package com.gdzy.elevator.data.sendfile;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

/**
 *  test send a file via socket
 */
public class TcpServerHelper {
    public TcpServerHelper() {
        initServerSocket();
    }

    private void initServerSocket() {
        try {

            ServerSocket serverSocket = null;
            serverSocket = new ServerSocket(8900);
            if(serverSocket==null){
                return;
            }
            //记录客户端的数量
            int count = 0;
            System.err.println("***服务器启动，等待客户端的连接***");
            //循环监听等待客户端的连接
            while (true) {
                Socket socket = null;
                //调用accept()方法开始监听，等待客户端的连接
                socket = serverSocket.accept();
                socket.setSoTimeout(60*1000);
                //创建一个新的线程  启动线程
//                new SendFileThread(socket).run();
                new Thread(new SendFileThread(socket)).start();
                // new Thread(new Task(socket)).start();
                count++;//统计客户端的数量
                System.out.println("客户端的数量：" + count);
                InetAddress address = socket.getInetAddress();
                System.out.println("当前客户端的IP：" + address.getHostAddress());
                System.out.println("当前本地的IP：" + socket.getLocalSocketAddress());
                System.out.println("当前本地的IP：" + socket.getLocalAddress());
            }

        } catch (IOException e) {
            e.printStackTrace();
        }


    }
}