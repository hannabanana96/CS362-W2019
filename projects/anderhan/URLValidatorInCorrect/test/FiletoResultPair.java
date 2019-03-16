import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.io.FileReader;
import java.util.List;
import java.util.Locale;

public class FiletoResultPair {
	public ResultPair[] readLines(String filename) throws IOException {
        FileReader fileReader = new FileReader(filename);
        BufferedReader bufferedReader = new BufferedReader(fileReader);
        List<ResultPair> Pairs = new ArrayList<ResultPair>();
        String line = null;
        while ((line = bufferedReader.readLine()) != null) {
        	ResultPair tempPair = new ResultPair(line.toLowerCase(Locale.ENGLISH), true);
            Pairs.add(tempPair);
        }
        bufferedReader.close();
        return Pairs.toArray(new ResultPair[Pairs.size()]);
    }
}
