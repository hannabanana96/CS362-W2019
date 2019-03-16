import static org.junit.Assert.*;

import org.junit.Test;
import java.io.File;
import java.util.Scanner;
import java.io.FileNotFoundException;


public class ProgramTest {

	@Test
	public void testIsValid() {
		UrlValidator fullVal_1 = new UrlValidator(null);
		assertTrue(fullVal_1.isValid("http://www.google.com"));

		/*** Test URLs from file ***/
		try {
			String url_file ="src/test_urls.txt";
			Scanner scanner = new Scanner(new File(url_file));

			while (scanner.hasNextLine()) {
				//Get URL and valid/invalid
				String[] parts = scanner.nextLine().split(",");

				//System.out.println(parts[0]);
				//System.out.println(parts[1]);

				if (parts[1].equals("valid")) {
					assertTrue(parts[0] + " expected to be " + parts[1],fullVal_1.isValid(parts[0]));
				}
				else if (parts[1].equals("invalid")) {
					assertTrue(parts[0] + " expected to be " + parts[1],!fullVal_1.isValid(parts[0]));
				}
				else {
					assertTrue(false);
				}
			} 
			scanner.close();
		}
		catch(FileNotFoundException ex) {
			System.out.println("File not found");
			assertTrue(false);
		}

		/*** Test Specific Cases ***/
		//test order
		assertTrue(!fullVal_1.isValid(":http//:65535www.google.com"));  //invalid
		assertTrue(!fullVal_1.isValid("www.google.com/http"));  		//invalid
		assertTrue(!fullVal_1.isValid("www.google.com/http:65535")); 	//invalid

		//test without schemes inputted
		assertTrue(fullVal_1.isValid("http://www.google.com:65535"));  	 	//valid
		assertTrue(fullVal_1.isValid("https://www.google.com:65535"));   	//valid
		assertTrue(fullVal_1.isValid("ftp://www.google.com:65535")); 	 	//valid
		assertTrue(!fullVal_1.isValid("httpss://www.google.com:65535"));  	//invalid

		//test with schemes inputted
		String[] schemes_2 = {"httpss"};
		UrlValidator fullVal_2 = new UrlValidator(schemes_2);
		assertTrue(!fullVal_2.isValid("http://www.google.com:65535"));  	//invalid
		assertTrue(!fullVal_2.isValid("https://www.google.com:65535"));   	//invalid
		assertTrue(!fullVal_2.isValid("ftp://www.google.com:65535")); 	 	//invalid
		assertTrue(fullVal_2.isValid("httpss://www.google.com:65535"));  	//valid

		//test authority options
		String[] schemes_3 = {"file","http"};
		UrlValidator fullVal_3 = new UrlValidator(schemes_3);
		assertTrue(fullVal_3.isValid("file://"));  	 							//valid
		assertTrue(!fullVal_3.isValid("file://:65535/hello?action=goodbye"));  	//invalid
		assertTrue(fullVal_3.isValid("file://www.google.com"));					//valid
		assertTrue(!fullVal_3.isValid("file:://www.google.com"));				//invalid

		//test with and without manual authorityvalidator
		RegexValidator regexval = new RegexValidator(new String[] {"hello"});
		UrlValidator fullVal4 = new UrlValidator(regexval, 0);
		assertTrue(fullVal4.isValid("http://hello/hi"));		//valid
		assertTrue(!fullVal4.isValid("http://goodbye/hi"));		//invalid

		//test allow all schemes
		long options_5 = UrlValidator.ALLOW_ALL_SCHEMES;
		UrlValidator fullVal5 = new UrlValidator(options_5);
		assertTrue(fullVal5.isValid("http://www.google.com:65535"));  		//valid
		assertTrue(fullVal5.isValid("https://www.google.com:65535"));   	//valid
		assertTrue(fullVal5.isValid("ftp://www.google.com:65535")); 	 	//valid
		assertTrue(fullVal5.isValid("httpss://www.google.com:65535"));  	//valid

		//test no fragments
		assertTrue(fullVal5.isValid("http://google.com:65535/#"));  		//fragment should be valid
		assertTrue(fullVal5.isValid("http://google.com:65535/#hello"));		//fragment should be valid
		assertTrue(fullVal5.isValid("http://google.com:65535/"));			//empty fragment should be valid
		long options_6 = UrlValidator.NO_FRAGMENTS;
		UrlValidator fullVal6 = new UrlValidator(options_6);
		assertTrue(!fullVal6.isValid("http://google.com:65535/#"));  		//fragment should not be valid
		assertTrue(!fullVal6.isValid("http://google.com:65535/#hello"));	//fragment should not be valid
		assertTrue(fullVal6.isValid("http://google.com:65535/"));			//no fragment should be valid

		//test allow two slashes
		assertTrue(fullVal6.isValid("http://google.com:65535/hello"));  	//one slash should be valid
		assertTrue(!fullVal6.isValid("http://google.com:65535//hello"));	//two slashes should not be valid
		assertTrue(fullVal6.isValid("http://google.com:65535/"));			//empty path should be valid
		long options_7 = UrlValidator.ALLOW_2_SLASHES;
		UrlValidator fullVal7 = new UrlValidator(options_7);
		assertTrue(fullVal7.isValid("http://google.com:65535/hello"));  	//one slash should be valid
		assertTrue(fullVal7.isValid("http://google.com:65535//hello"));	//two slashes should not be valid
		assertTrue(fullVal7.isValid("http://google.com:65535/"));			//empty path should be valid
	}

	@Test
	public void testIsValidScheme() {
		//Test default schemes
		UrlValidator schemeVal1 = new UrlValidator(null);
		assertTrue(schemeVal1.isValidScheme("http"));  		//default scheme should be valid
		assertTrue(schemeVal1.isValidScheme("https"));  	//default scheme should be valid
		assertTrue(schemeVal1.isValidScheme("ftp"));		//default scheme should be valid
		assertTrue(!schemeVal1.isValidScheme(""));			//empty scheme should not be valid
		assertTrue(!schemeVal1.isValidScheme(null));		//null scheme should not be valid
		assertTrue(!schemeVal1.isValidScheme("tfp"));		//should not be valid
		assertTrue(!schemeVal1.isValidScheme("httpss"));	//should not be valid

		//Test scheme as input to UrlValidator
		String[] schemes = {"httpss"};
		UrlValidator schemeVal2 = new UrlValidator(schemes);
		assertTrue(!schemeVal2.isValidScheme("http"));  	//default scheme should be valid
		assertTrue(!schemeVal2.isValidScheme("https"));  	//default scheme should be valid
		assertTrue(!schemeVal2.isValidScheme("ftp"));		//default scheme should be valid
		assertTrue(!schemeVal2.isValidScheme(""));			//empty scheme should not be valid
		assertTrue(!schemeVal1.isValidScheme(null));		//null scheme should not be valid
		assertTrue(!schemeVal2.isValidScheme("tfp"));		//should not be valid
		assertTrue(schemeVal2.isValidScheme("httpss"));		//inputted scheme should be valid

		//Test allow_all_schemes
		long options = UrlValidator.ALLOW_ALL_SCHEMES;
		UrlValidator schemeVal3 = new UrlValidator(options);
		assertTrue(schemeVal3.isValidScheme("http"));  		//all schemes should be valid
		assertTrue(schemeVal3.isValidScheme("https"));  	//all schemes should be valid
		assertTrue(schemeVal3.isValidScheme("ftp"));		//all schemes should be valid
		assertTrue(!schemeVal3.isValidScheme(""));			//all schemes should be valid, except empty
		assertTrue(!schemeVal1.isValidScheme(null));		//all schemes should be valid, except null
		assertTrue(schemeVal3.isValidScheme("tfp"));		//all schemes should be valid
		assertTrue(schemeVal3.isValidScheme("httpss"));		//all schemes should be valid	
	}

	@Test
	public void testIsValidAuthority() {
		//Test default 
		UrlValidator authVal1 = new UrlValidator(null);		
		assertTrue(!authVal1.isValidAuthority(""));		//empty authority should not be valid
		assertTrue(!authVal1.isValidAuthority(null));		//null authority should not be valid

		//valid ipv6 authorties
		assertTrue(authVal1.isValidAuthority("[562B:2200:D465:AAAA:BBBB:9999:0123:4444]"));		//valid ipv6
		assertTrue(authVal1.isValidAuthority("[562B:2200:D465:AAAA:BBBB:9999:0123:4444]:80"));	//valid port
		assertTrue(authVal1.isValidAuthority("[562B:22:D465:AAAA:BB:9999:0:4444]:2"));			//ok to compress zeors, valid port
		assertTrue(authVal1.isValidAuthority("[562B:22:D465:AA:BB:99:0123:44]"));				//ok to compress zeros
		assertTrue(authVal1.isValidAuthority("[562B:22:D465:AA::99:0123:44]"));					//can have :: in middle
		assertTrue(authVal1.isValidAuthority("[562B:22:D465:AA:BB:99::44]"));					//can have :: in middle
		assertTrue(authVal1.isValidAuthority("[562B:22:D465:AA:BB:99:0123::]"));				//can end with ::
		assertTrue(authVal1.isValidAuthority("[::BB:AA:BB:99:0123:45]"));						//can start with ::

		//invalid ipv6 authorites
		assertTrue(!authVal1.isValidAuthority("[562B:220o:D465:AAAA:BBBB:9999:0123:4444]"));		//contains letter o	
		assertTrue(!authVal1.isValidAuthority("[562B:2200:D465:AAAA:BBBB:9999:0123:4444]:-1"));		//invalid port
		assertTrue(!authVal1.isValidAuthority("[562B:22:D465:AA:BB:99:0123::44]"));					//9 hex groups
		assertTrue(!authVal1.isValidAuthority("[562B:22:D465:AA:BB:99:0123:]"));					//invalid
		assertTrue(!authVal1.isValidAuthority("[562B:22::::BB:99:0123]"));							//invalid
		assertTrue(!authVal1.isValidAuthority("[:22:BB:AA:BB:99:0123:45]"));						//cannot start with :
		assertTrue(!authVal1.isValidAuthority("[562B:22:D465:AA:BB:99:0123:BB:]"));					//cannot end with :
		assertTrue(!authVal1.isValidAuthority("[562BAB:22:D465:AA:BB:99:0123:BB:]"));				//too many hex digits for group
		assertTrue(!authVal1.isValidAuthority("[D465:AA:BB:99:0123:44:-1]"));						//negative number

		//valid ipv4
		assertTrue(authVal1.isValidAuthority("0.0.0.0"));			//valid ipv4
		assertTrue(authVal1.isValidAuthority("255.255.255.255"));	//valid ipv4
		assertTrue(authVal1.isValidAuthority("0.0.0.0:20"));		//valid port
		assertTrue(authVal1.isValidAuthority("0.0.0.0:65535"));		//valid port

		//invalid ipv4
		assertTrue(!authVal1.isValidAuthority("256.256.256.256"));	//invalid ipv4
		assertTrue(!authVal1.isValidAuthority("-1.-1.-1.-1"));		//invalid ipv4
		assertTrue(!authVal1.isValidAuthority("05.0.0.0"));			//invalid ipv4
		assertTrue(!authVal1.isValidAuthority("0.0.0.0:-1"));		//invalid port
		assertTrue(!authVal1.isValidAuthority("0.0.0.0:65536"));	//invalid port

		//Local Host
		assertTrue(!authVal1.isValidAuthority("localhost"));	//local hosts not allowed
		assertTrue(!authVal1.isValidAuthority("machine"));		//local hosts not allowed

		//Local Host allow local urls
		long options = UrlValidator.ALLOW_LOCAL_URLS;
		UrlValidator authVal2 = new UrlValidator(options);
		assertTrue(authVal2.isValidAuthority("localhost"));		//valid local url
		assertTrue(authVal2.isValidAuthority("machine"));		//valid local url
		assertTrue(authVal2.isValidAuthority("localhost:80"));	//valid local url and port
		assertTrue(authVal2.isValidAuthority("machine:8888"));	//valid local url and port
		assertTrue(!authVal2.isValidAuthority("machine:-1"));	//invalid port

		//manual authorityvalidator match all authorities
		RegexValidator regexval_3 = new RegexValidator(new String[] {".*"});
		UrlValidator authVal3 = new UrlValidator(regexval_3, 0);
		assertTrue(authVal3.isValidAuthority("hello"));
		assertTrue(authVal3.isValidAuthority("goodbye"));

		//manual authorityvalidator match one specific authority
		RegexValidator regexval_4 = new RegexValidator(new String[] {"hello"});
		UrlValidator authVal4 = new UrlValidator(regexval_4, 0);
		assertTrue(authVal4.isValidAuthority("hello"));
		assertTrue(!authVal4.isValidAuthority("goodbye"));	
	}

	@Test
	public void testIsValidPath() {
		//Default options
		UrlValidator pathVal1 = new UrlValidator(null);
		assertTrue(!pathVal1.isValidPath("hello"));  	//no slashes should not be valid
		assertTrue(pathVal1.isValidPath("/hello"));  	//one slash should be valid
		assertTrue(!pathVal1.isValidPath("//hello"));	//two slashes should not be valid
		assertTrue(pathVal1.isValidPath(""));			//empty path should be valid
		assertTrue(!pathVal1.isValidPath(null));		//null path should not be valid
		assertTrue(!pathVal1.isValidPath("#"));			//fragment should not be valid
		assertTrue(!pathVal1.isValidPath("#goodbye"));	//fragment should not be valid

		//Allow 2 slashes
		long options_1 = UrlValidator.ALLOW_2_SLASHES;
		UrlValidator pathVal2 = new UrlValidator(options_1);
		assertTrue(!pathVal2.isValidPath("hello"));  	//no slashes should not be valid
		assertTrue(pathVal2.isValidPath("/hello"));  	//one slash should be valid
		assertTrue(pathVal2.isValidPath("//hello"));	//two slashes should not be valid
		assertTrue(pathVal2.isValidPath(""));			//empty path should be valid
		assertTrue(!pathVal2.isValidPath(null));		//null path should not be valid
	}

	@Test
	public void testIsValidQuery() {
		//Default options
		UrlValidator queryVal1 = new UrlValidator(null);
		assertTrue(queryVal1.isValidQuery("hello"));  			//query should be valid
		assertTrue(queryVal1.isValidQuery("?hello"));  			//query should be valid
		assertTrue(queryVal1.isValidQuery("?hello=goodbye"));	//query should be valid
		assertTrue(queryVal1.isValidQuery("#"));				//query should be valid
		assertTrue(queryVal1.isValidQuery(""));					//empty query should be valid
		assertTrue(queryVal1.isValidQuery(null));				//null query should be valid
	}

	@Test
	public void testIsValidFragment() {
		//Default options
		UrlValidator fragVal1 = new UrlValidator(null);
		assertTrue(fragVal1.isValidFragment("hello")); 	//fragment should be valid
		assertTrue(fragVal1.isValidFragment("#"));  	//fragment should be valid
		assertTrue(fragVal1.isValidFragment("#hello"));	//fragment should be valid
		assertTrue(fragVal1.isValidFragment(""));		//empty fragment should be valid
		assertTrue(fragVal1.isValidFragment(null));		//null fragment should be valid


		//Disallow fragments
		long options = UrlValidator.NO_FRAGMENTS;
		UrlValidator fragVal2 = new UrlValidator(options);
		assertTrue(!fragVal2.isValidFragment("hello"));	 	//fragment should not be valid
		assertTrue(!fragVal2.isValidFragment("#"));  		//fragment should not be valid
		assertTrue(!fragVal2.isValidFragment("#hello"));	//fragment should not be valid
		assertTrue(!fragVal2.isValidFragment(""));			//empty fragment should not be valid
		assertTrue(fragVal2.isValidFragment(null));			//null fragment should be valid
	}


}
