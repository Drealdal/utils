package test;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class server {

	public static void main(String args[])
	{
		try {
			ServerSocket server=new ServerSocket(1234);
			Socket socket = server.accept();
			
			DataInputStream in =new DataInputStream(socket.getInputStream());
			DataOutputStream out = new DataOutputStream(socket.getOutputStream());
			int id = in.readInt();
			System.out.println("Id: " + id);
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
