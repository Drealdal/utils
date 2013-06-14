package client;

import java.io.File;
import common.spliter;

public class diffclient {

	public static int usage(String []args)
	{
		if( args.length < 4)
		{
			System.out.print(": src node:port:dest");
			return -1;
		}
		return 0;
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		if( usage(args) < 0 )
		{
			return;
		}
		String src = args[0];
		String destString = args[1];
		String dests[] = destString.split(":");
		String serverName = dests[0];
		String port = dests[1];
	
		String destfile = dests[2];
		File input = new File(src);
	
		if( input.isFile() )
		{
			new spliter(serverName,Short.parseShort(port),input.getAbsolutePath(),destfile).run();
		}else if(input.isDirectory())
		{
			System.out.print("Copy of directory is not allowed");
			return;
		}
	}

}
