package com.gdzy.elevator.data.sendfile;

public class App {
    /**
     * 入口
     *
     * @param args
     */
    public static void main(String[] args) {
        new TcpServerHelper();

    }

    public static byte[] intTo2ByteArray(int a) {
        return new byte[] {
                (byte) ((a >> 8) & 0xFF),
                (byte) (a & 0xFF)
        };
    }
    public static void printByteArrayToHex(byte[] a) {
        String hex;
        for (int i=0;i<a.length;i++){
            hex = Integer.toHexString(a[i] & 0xFF);
            if (hex.length() < 2) {
                hex = "0" + hex;
            }
            System.out.print(hex );
            System.out.print(" " );
        }
        System.out.println(  );
        System.out.println("-------------------" );
    }
    public static byte[] intTo4ByteArray(int a) {
        return new byte[] {
                (byte) ((a >> 24) & 0xFF),
                (byte) ((a >> 16) & 0xFF),
                (byte) ((a >> 8) & 0xFF),
                (byte) (a & 0xFF)
        };
    }
}
