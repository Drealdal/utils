package common;
import java.util.Vector;
import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.net.Socket;
import java.net.UnknownHostException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class spliter implements Runnable{

	private String serverName = null;
	private short port = 0;
	private int totalsend = 0;
	private long totalchunk = 0;
	static int chunksize = 4096;
	private String fname = null;
	private String tname = null;
	RandomAccessFile  file = null; 
	ArrayList<chunk> chunks = new ArrayList<chunk>();
	private ExecutorService exeReader = Executors.newCachedThreadPool();
	private ExecutorService exeSender = Executors.newCachedThreadPool();
	public spliter(String serverName, short port,String fname,String tname )
	{
		this.serverName = serverName;
		this.port = port;
		this.fname = fname;
		this.tname = tname;
	}
	class chunk{
		private long  off;
		private int len = 0;
		private byte data[] = null;
		private byte digest[] = null;
		int id;
		
		public long getOff() {
			return off;
		}


		public void setOff(long off) {
			this.off = off;
		}


		public int getLen() {
			return len;
		}


		public void setLen(int len) {
			this.len = len;
		}


		public byte[] getData() {
			return data;
		}


		public void setData(byte[] data) {
			this.data = data;
		}


		public byte[] getDigest() {
			return digest;
		}


		public void setDigest(byte[] digest) {
			this.digest = digest;
		}


		chunk(int id){
			this.off = id*spliter.chunksize;
			this.id = id;
			int rest = 0;
			try {
				rest = (int) (file.length() - this.off);
				len = rest > chunksize ? chunksize:rest; 
				System.out.print("Length:"+len);
				data = new byte[len];
				
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			
			synchronized(file){
				try {
					file.read(data);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		
	
		public int getId() {
			return id;
		}


		public void setId(int id) {
			this.id = id;
		}


		public int CheckSum()
		{
			try {
				MessageDigest md = MessageDigest.getInstance("MD5");
				md.update(data);
				digest = md.digest();
			} catch (NoSuchAlgorithmException e) {
				System.out.print("Unknown Error: not find MD5");
				return -1;
			}
			return 0;
		}

		
	}

	class reader implements Runnable {
		

		@Override
		public void run() {
			// TODO Auto-generated method stub
			try {
				file = new RandomAccessFile(fname,"r");
				if(file == null )
				{
					System.out.print("输入文件不存在！");
					return;
				}
				long numofchunks = file.length()/spliter.chunksize;
				if( file.length() % spliter.chunksize != 0 )
				{
					numofchunks ++;
				}
				System.out.print("Total Number to send:"+numofchunks);
				totalchunk = numofchunks;
				
				for( int i = 0 ; i < numofchunks; i++)
				{
					chunk nchunk = new chunk(i);
					exeSender.execute(new sender(nchunk));
					//new sender(nchunk).run();
				}
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}

	class sender implements Runnable {

		chunk ch = null;
		sender(chunk chu)
		{
			this.ch = chu;
		}
		@Override
		public void run() {
			// TODO Auto-generated method stub
			chunk ctosend = this.ch;
			ctosend.CheckSum();
			Socket socket = null;
			try {
				socket = new Socket(serverName, port);
				DataInputStream in = new DataInputStream(socket.getInputStream());
				DataOutputStream out = new DataOutputStream(socket.getOutputStream());
				out.write((fname+":"+tname).getBytes());
				in.readBoolean();
				System.out.println("Id:"+ctosend.getId());
				out.writeInt(ctosend.getId());
				//out.flush();
				out.write(ctosend.getDigest(),0,ctosend.getDigest().length);
				boolean send = in.readBoolean();
				if(send)
				{
					totalsend++;
					System.out.print("Sending "+ctosend.getId()+"...");
					out.writeLong(ctosend.getOff());
					out.writeInt(ctosend.getLen());
					out.write(ctosend.getData(),0,ctosend.getData().length);
				}
				
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally{
				try {
					socket.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			
		}
		
	}
	/**
	 * @param args
	 * @throws NoSuchAlgorithmException 
	 */
	public static void main(String[] args) throws NoSuchAlgorithmException {
		
		spliter sp = new spliter("localhost",(short) 0x0707,"/home/foryee/cp/a.c","/home/foryee/to/a.c");
		sp.run();
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		reader r = new reader();
		r.run();
	}

}
