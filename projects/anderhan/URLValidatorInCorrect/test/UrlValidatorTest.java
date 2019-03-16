import junit.framework.TestCase;
import java.util.Random;
import java.util.ArrayList;
import java.util.List;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Scanner;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!

public class UrlValidatorTest extends TestCase {

	public UrlValidatorTest(String testName) {
		super(testName);
	}	


	public void testManualTest()
	{

		String exit = "exit";

		//continue checking inputs
		while(true) {

			System.out.println("****New Test****");

			//get URL input from user
			System.out.println("Enter a url to test: ");
			Scanner s = new Scanner(System.in);
			String other_str = s.next();

			//if user inputed "exit"
			if(exit.compareTo(other_str) == 0) {
				System.out.println("Exiting");
				break;
			}

			//set scheme to test specific scheme
			//String[] scheme = {"http"};
			//set to 8 for local authority
			//set to 4 for no fragments
			//set to 2 for allow_2_slashes
			//set to 1 for all_all_schemes
			UrlValidator urlValidator = new UrlValidator(4);

			//checks if the input was a valid url
			if (urlValidator.isValid(other_str)) {
				System.out.println("PASS: url is valid");
				System.out.println(other_str);
			}
			else {
				System.out.println("FAIL: url is invalid");
				System.out.println(other_str);
			}  
		}  
	}

	public Boolean partition1PassFail = true;

	public Boolean partition2PassFail = true;

	public Boolean partition3PassFail = true;

	public Boolean partition4PassFail = true;

	public Boolean partition5PassFail = true;


	public List<ResultPair> testScheme = new ArrayList<ResultPair>();

	public List<ResultPair> testAuthority = new ArrayList<ResultPair>();

	public List<ResultPair> testPort = new ArrayList<ResultPair>();

	public List<ResultPair> testPath = new ArrayList<ResultPair>();

	public List<ResultPair> testQuery = new ArrayList<ResultPair>();

	private static final int numTestSchemes = 1000000;
	private static final int numTestAuthorities = 1000000;
	private static final int numTestPaths = 1000000;
	private ResultPair[] testUrlQuery = {new ResultPair("?action=view", true),
		new ResultPair("?action=edit&mode=up", true),
		new ResultPair("", true),
		new ResultPair(":Bad_Query", false),
		new ResultPair("invalid", false),
	};

	public ResultPair[] schemeInvalidText = {
		new ResultPair("hello", false),	
		new ResultPair("world", false),	
		new ResultPair("notvalid", false),	
		new ResultPair("fake", false),	
		new ResultPair("12345", false),	
		new ResultPair("testestest", false),	
		new ResultPair("foo", false),	
		new ResultPair("bar", false),	
		new ResultPair("example", false),	
		new ResultPair("98765", false),	
		new ResultPair("cat", false)	
	};

	public ResultPair[] schemeSuffix = {
		new ResultPair("", false),
		new ResultPair("://", true),
		new ResultPair("://", true),
		new ResultPair("://", true),
		new ResultPair(":", false),
		new ResultPair(":/", false),
		new ResultPair("//", false),
		new ResultPair("/", false)
	};

	public ResultPair[] authorityInvalidTld = {
		new ResultPair("coin", false),
		new ResultPair("hello", false),
		new ResultPair("thisisnotatld", false),
		new ResultPair("invalid", false),
		new ResultPair("bad", false),
		new ResultPair("negatory", false),
		new ResultPair("nogo", false),
		new ResultPair("ixnay", false),
		new ResultPair("404", false)
	};

	public ResultPair[] pathTokens = {
		new ResultPair("hello", true),
		new ResultPair("12345", true),
		new ResultPair("fd4x523", true),
		new ResultPair("f-o!o", true),
		new ResultPair("b~@r", true),
		new ResultPair("tes__.t", true),
		new ResultPair("x(a+b*c*)y", true),
		new ResultPair("this=is,a$valid;string&token", true),
		new ResultPair("hello/", false),
		new ResultPair("^invalid", false),
		new ResultPair("not acceptable", false),
		new ResultPair("`incorrect`", false),
		new ResultPair("[nogo]", false),
		new ResultPair("{unaccaptable}", false)
	};

	public void setup() throws IOException {
		Random rand = new Random();
		FiletoResultPair fileParser = new FiletoResultPair();
		ResultPair[] schemeValidText = fileParser.readLines("test/schemeList.txt");
		ResultPair[] authorityValidTld = fileParser.readLines("test/TldList.txt");
		ResultPair tempPair = new ResultPair("http://", true);
		testScheme.add(tempPair);
		tempPair = new ResultPair("https://", true);
		testScheme.add(tempPair);
		tempPair = new ResultPair("ftp://", true);
		testScheme.add(tempPair);
		tempPair = new ResultPair("h3t://", true);
		testScheme.add(tempPair);
		tempPair = new ResultPair("HTTP://", true);
		testScheme.add(tempPair);
		for(int i = 0; i < numTestSchemes; i++) {
			int suffixInd = rand.nextInt(schemeSuffix.length);
			int textInd = rand.nextInt(schemeValidText.length);
			if (schemeValidText[ textInd ].item.equals("file")) {
				if (schemeSuffix[ suffixInd ].item.equals(":/")) {
					tempPair = new ResultPair(schemeValidText[ textInd ].item + schemeSuffix[ suffixInd ].item, true);
				}
				else {
					tempPair = new ResultPair(schemeValidText[ textInd ].item + schemeSuffix[ suffixInd ].item, false);
				}
			}
			else {
				tempPair = new ResultPair(schemeValidText[ textInd ].item + schemeSuffix[ suffixInd ].item, schemeValidText[ textInd ].valid && schemeSuffix[ suffixInd ].valid);
			}
			testScheme.add(tempPair);

		}


		tempPair = new ResultPair("" + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 1), true);
		testAuthority.add(tempPair);
		tempPair = new ResultPair("0.0.0.0", true);
		testAuthority.add(tempPair);
		tempPair = new ResultPair("255.255.255.255", true);
		testAuthority.add(tempPair);
		tempPair = new ResultPair("" + String.valueOf(rand.nextInt(255) + 256) + "." + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 1), false);
		testAuthority.add(tempPair);
		tempPair = new ResultPair("" + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 256) + "." + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 1), false);
		testAuthority.add(tempPair);
		tempPair = new ResultPair("" + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 256) + "." + String.valueOf(rand.nextInt(255) + 1), false);
		testAuthority.add(tempPair);
		tempPair = new ResultPair("" + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 1) + "." + String.valueOf(rand.nextInt(255) + 256), false);
		testAuthority.add(tempPair);
		String tempStr = "g";
		for (int i = 0; i < 250; i++) {
			tempStr = tempStr + "o";
		}
		tempStr = tempStr + "gle.com";
		tempPair = new ResultPair(tempStr, false);
		for (int i = 0; i < numTestAuthorities; i++) {
			int badGood = rand.nextInt(2);
			int twoOrThree = rand.nextInt(2);
			if (twoOrThree == 1) {
				tempStr = "www.";
			}
			else {
				tempStr = "";
			}
			for (int j = 0; j < rand.nextInt(50) + 1; j++) {
				tempStr = tempStr + (char)(rand.nextInt(26) + 'a');
			}
			if (badGood == 0) {
				tempStr = tempStr + "." + authorityInvalidTld[ rand.nextInt(authorityInvalidTld.length) ].item;
				tempPair = new ResultPair(tempStr, false);
			}
			else {
				tempStr = tempStr + "." + authorityValidTld[ rand.nextInt(authorityValidTld.length) ].item;
				tempPair = new ResultPair(tempStr, true);
			}
			testAuthority.add(tempPair);
		}


		tempPair = new ResultPair(":" + String.valueOf(rand.nextInt(65536)), true);
		testPort.add(tempPair);
		tempPair = new ResultPair(":0", true);
		testPort.add(tempPair);
		tempPair = new ResultPair(":65535", true);
		testPort.add(tempPair);
		tempPair = new ResultPair(":" + String.valueOf(rand.nextInt(65536) + 65536), false);
		testPort.add(tempPair);
		tempPair = new ResultPair(":" + String.valueOf(0 - rand.nextInt(65536)), false);
		testPort.add(tempPair);


		for (int i = 0; i < numTestPaths; i++) {
			int numTokens = rand.nextInt(10);
			if (numTokens == 0) {
				tempPair = new ResultPair("", true);
				testPath.add(tempPair);
			}
			else {
				tempStr = "/";
				Boolean tempBool = true;
				for (int j = 0; j < numTokens; j++) {
					int tempInd = rand.nextInt(pathTokens.length);
					tempStr = tempStr + pathTokens[ tempInd ].item + "/";
					tempBool = tempBool && pathTokens[ tempInd ].valid;
				}
				tempPair = new ResultPair(tempStr, tempBool);
				testPath.add(tempPair);
			}
		}

	}

	public void testYourFirstPartition()
	{
		ResultPair[] testUrlScheme = testScheme.toArray(new ResultPair[testScheme.size()]);
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		String baseToken1 = "";
		String baseToken2 = "www.google.com:80/test1?action=view";
		for(int i = 0; i < testUrlScheme.length; i++) {
			ResultPair testToken = testUrlScheme[ i ];
			String testStr = baseToken1 + testToken.item + baseToken2;
			Boolean result = urlVal.isValid(testStr);
			Boolean expected = testToken.valid;
			String notification = "Testing \"" + testStr + "\"";
			try {
				assertEquals(testToken.item, result, expected);
			}	catch(Throwable t) {
				System.out.println(notification);
				String outStr = "Failure: Expected " + expected.toString() + " got " + result.toString() + ".\n";
				System.out.println(outStr);
				partition1PassFail = false;
			}
		}
	}

	public void testYourSecondPartition()
	{
		ResultPair[] testUrlAuthority = testAuthority.toArray(new ResultPair[testAuthority.size()]);
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		String baseToken1 = "http://";
		String baseToken2 = "/test1?action=view";
		for(int i = 0; i < testUrlAuthority.length; i++) {
			ResultPair testToken = testUrlAuthority[ i ];
			String testStr = baseToken1 + testToken.item + baseToken2;
			Boolean result = urlVal.isValid(testStr);
			Boolean expected = testToken.valid;
			String notification = "Testing \"" + testStr + "\"";
			try {
				assertEquals(testToken.item, result, expected);
			}	catch(Throwable t) {
				System.out.println(notification);
				String outStr = "Failure: Expected " + expected.toString() + " got " + result.toString() + ".\n";
				System.out.println(outStr);
				partition2PassFail = false;
			}
		}
	}

	public void testYourThirdPartition()
	{
		ResultPair[] testUrlPort = testPort.toArray(new ResultPair[testPort.size()]);
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		String baseToken1 = "http://www.google.com";
		String baseToken2 = "/test1?action=view";
		for(int i = 0; i < testUrlPort.length; i++) {
			ResultPair testToken = testUrlPort[ i ];
			String testStr = baseToken1 + testToken.item + baseToken2;
			Boolean result = urlVal.isValid(testStr);
			Boolean expected = testToken.valid;
			String notification = "Testing \"" + testStr + "\"";
			try {
				assertEquals(testToken.item, result, expected);
			}	catch(Throwable t) {
				System.out.println(notification);
				String outStr = "Failure: Expected " + expected.toString() + " got " + result.toString() + ".\n";
				System.out.println(outStr);
				partition3PassFail = false;
			}
		}
	}

	public void testYourFourthPartition()
	{
		ResultPair[] testUrlPath = testPath.toArray(new ResultPair[testPath.size()]);
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		String baseToken1 = "http://www.google.com";
		String baseToken2 = "?action=view";
		for(int i = 0; i < testUrlPath.length; i++) {
			ResultPair testToken = testUrlPath[ i ];
			String testStr = baseToken1 + testToken.item + baseToken2;
			Boolean result = urlVal.isValid(testStr);
			Boolean expected = testToken.valid;
			String notification = "Testing \"" + testStr + "\"";
			try {
				assertEquals(testToken.item, result, expected);
			}	catch(Throwable t) {
				System.out.println(notification);
				String outStr = "Failure: Expected " + expected.toString() + " got " + result.toString() + ".\n";
				System.out.println(outStr);
				partition4PassFail = false;
			}
		}
	}

	public void testYourFifthPartition()
	{
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		String baseToken1 = "http://www.google.com";
		String baseToken2 = "";
		for(int i = 0; i < testUrlQuery.length; i++) {
			ResultPair testToken = testUrlQuery[ i ];
			String testStr = baseToken1 + testToken.item + baseToken2;
			Boolean result = urlVal.isValid(testStr);
			Boolean expected = testToken.valid;
			String notification = "Testing \"" + testStr + "\"";
			try {
				assertEquals(testToken.item, result, expected);
			}	catch(Throwable t) {
				System.out.println(notification);
				String outStr = "Failure: Expected " + expected.toString() + " got " + result.toString() + ".\n";
				System.out.println(outStr);
				partition5PassFail = false;
			}
		}
	}

	//You need to create more test cases for your Partitions if you need to 


	/**
	 * Only used to debug the unit tests.
	 * @param argv
	 * @throws IOException 
	 */
	public static void main(String[] argv) throws IOException {

		UrlValidatorTest fct = new UrlValidatorTest("url test");
		fct.setup();
		System.out.println("Testing Schemes");
		fct.testYourFirstPartition();
		if (!fct.partition1PassFail) {
			System.out.println("Failed");
		}
		else {
			System.out.println("Passed!");
		}
		System.out.println("Testing Authorities");
		fct.testYourSecondPartition();
		if (!fct.partition2PassFail) {
			System.out.println("Failed");
		}
		else {
			System.out.println("Passed!");
		}
		System.out.println("Testing Ports");
		fct.testYourThirdPartition();
		if (!fct.partition3PassFail) {
			System.out.println("Failed");
		}
		else {
			System.out.println("Passed!");
		}
		System.out.println("Testing Paths");
		fct.testYourFourthPartition();
		if (!fct.partition4PassFail) {
			System.out.println("Failed");
		}
		else {
			System.out.println("Passed!");
		}
		System.out.println("Testing Queries");
		fct.testYourFifthPartition();
		if (!fct.partition5PassFail) {
			System.out.println("Failed");
		}
		else {
			System.out.println("Passed!");
		}
	}
}
