package test;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

public class client {

	public static void main(String args[])
	{
		try {
			Socket socket = new Socket("127.0.0.1",1234);
			DataInputStream in = new DataInputStream(socket.getInputStream());
			DataOutputStream out =new DataOutputStream(socket.getOutputStream());
			out.writeInt(1);
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
