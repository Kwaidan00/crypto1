import java.io.File;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.regex.Pattern;
import javax.crypto.*;
import javax.crypto.spec.*;

public class zad2
{

	private static boolean isprintable(int[] str)
	{
		for (int i = 0; i < 16; i++)
		{
			if (str[i] < 32 || str[i] > 126)
				return false;
		}
		return true;
	}
	public static void main(String[] args)
	{
		String half_key;
		String cString;
		half_key = args[args.length-1];

		try
		{
			cString = new String(Files.readAllBytes(Paths.get(args[args.length-2])));
			System.out.println("MSG: " + cString);
			System.out.println("Half of key: " + half_key);

			String[] parts = cString.split(" ");
			byte[] c2 = new byte[parts.length];
			for (int i = 0; i < parts.length; i++)
			{
				c2[i] = (byte)Integer.parseInt(parts[i].substring(0,8), 2);
			}
			System.out.println("MSG as int:");
			for (int i = 0; i < c2.length; i++)
			{
				System.out.print(c2[i] + " ");
			}
			byte[] half_key2 = new byte[half_key.length()];
			for (int i = 0; i < half_key.length(); i++)
			{
				half_key2[i] = (byte)Integer.parseInt(half_key.substring(i, i+1), 16);
			}
			for (int i = 0; i < half_key2.length; i++)
			{
				System.out.print(half_key2[i] + " ");
			}

			String resultString;
			SecretKeySpec key;
			Cipher decrypt;
			byte[] result = new byte[16];
			byte[] table = new byte[16];
			table[0] = 0;
			table[1] = 0;
			table[2] = 0;
			table[3] = 0;
			table[4] = 0;
			table[5] = 0;
			table[6] = 0;
			table[7] = 0;
			table[8] = half_key2[0];
			table[9] = half_key2[1];
			table[10] = half_key2[2];
			table[11] = half_key2[3];
			table[12] = half_key2[4];
			table[13] = half_key2[5];
			table[14] = half_key2[6];
			table[15] = half_key2[7];

			for (int a = 0; a < 16; a++)
				for (int b = 0; b < 16; b++)
					for (int c = 0; c < 16; c++)
						for (int d = 0; d < 16; d++)
							for (int e = 0; e < 16; e++)
								for (int f = 0; f < 16; f++)
									for (int g = 0; g < 16; g++)
										for (int h = 0; h < 16; h++)
										{
											table[0] = (byte)a;
											table[1] = (byte)b;
											table[2] = (byte)c;
											table[3] = (byte)d;
											table[4] = (byte)e;
											table[5] = (byte)f;
											table[6] = (byte)g;
											table[7] = (byte)h;
											key = new SecretKeySpec(table, "RC4");
											decrypt = Cipher.getInstance("RC4");
											decrypt.init(Cipher.DECRYPT_MODE, key);
											resultString = new String(decrypt.update(c2), "ASCII");
											if ( Pattern.matches("[a-zA-Z0-9 .?,!()@\"'%-+]+", resultString) )
											{
												System.out.println("Pasuje moze " + resultString + " dla klucza " + table.toString());
											}
										}


		}
		catch (Exception e)
		{
			System.out.println("Error");
		}
	}
}
