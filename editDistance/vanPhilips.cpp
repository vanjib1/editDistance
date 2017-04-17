
/*Author          : J.Brandon Van Philips
 *Date            : 4/17/17
 *Assignment      : Dynamic Programming—Edit Distance
 *File            : vanPhilips.cpp
 *Honor Statement : I have neither given nor received any unauthorized help on this assignment.
 *resources       : 
 *	http://stackoverflow.com/questions/12375591/vector-of-vectors-to-create-matrix
 *	http://leojiang.com/experiments/levenshtein/
 *	http://computing.southern.edu/halterman/Courses/Winter2017/318/Assignments/editdistance.html
 *	http://computing.southern.edu/halterman/Courses/Winter2017/318/s673562/Slides/edit-distance-handout.pdf
 *	
 */


#include <iostream>
#include <string>
#include <vector>

std::vector<std::vector<int>> compareString(std::string workingString, std::string comparisonString);
int getEditDistance(std::vector<std::vector<int>> matrix);
std::string editOperations(std::vector<std::vector<int>> matrix, std::string workingString, std::string comparisonString);


int main()
{
	std::string workingString;
	std::string comparisonString;
	do
	{
		std::cout << "Turn: ";
		getline(std::cin, workingString);
		if (workingString == "")
			break;
		std::cout << "into: ";
		getline(std::cin, comparisonString);

		std::vector<std::vector<int>> matrix = compareString(workingString, comparisonString);
		std::cout << getEditDistance(matrix) << ":";
		std::cout << editOperations(matrix, workingString, comparisonString) << std::endl;
	}
	while (workingString != "");


	return 0;
}

/*creates a Levenshtein distance matrix using a workingString and a comparisonString. 
 * workingString -> comparisonString 
 */
std::vector<std::vector<int>> compareString(std::string workingString, std::string comparisonString)
{
	/*matrix layout
	*
	*   c o m p a r i s o n
	* w 
	* o 
	* r
	* k
	* i
	* n
	* g
	*/

	std::vector<std::vector<int>> matrix(comparisonString.size() + 1, std::vector<int>(workingString.size() + 1));

	for (auto i = 0; i < matrix.size(); i++) //comparison string set up
	{
		matrix[i][0] = i;
	}

	for (auto i = 0; i < matrix[0].size(); i++) // working string set up
	{
		matrix[0][i] = i;
	}

	for (auto y = 1; y < matrix[0].size(); y++) //moves vertically through
	{
		for (auto x = 1; x < matrix.size(); x++) //moves horizontally
		{
			if (comparisonString[x - 1] == workingString[y - 1])
			{
				matrix[x][y] = matrix[x - 1][y - 1];
			}
			else
			{
				if ((matrix[x - 1][y] <= matrix[x - 1][y - 1]) && matrix[x - 1][y] <= matrix[x][y - 1])
				{
					matrix[x][y] = matrix[x - 1][y] + 1;
				}
				if (matrix[x - 1][y - 1] <= matrix[x - 1][y] && (matrix[x - 1][y - 1] <= matrix[x][y - 1]))
				{
					matrix[x][y] = matrix[x - 1][y - 1] + 1;
				}
				if ((matrix[x][y - 1] <= matrix[x - 1][y]) && (matrix[x][y - 1] <= matrix[x - 1][y - 1]))
				{
					matrix[x][y] = matrix[x][y - 1] + 1;
				}
			}
		}
	}
	return matrix;
}

//Uses a Levenshtein distance matrix to calculate the edit distance between two strings
int getEditDistance(std::vector<std::vector<int>> matrix)
{
	return matrix[matrix.size() - 1][matrix[0].size() - 1];
}

/*Uses a Levenshtein distance matrix, workingString, and CompareString to calculate the operations needed to turn workingString into comparisonString.
 * It returns a string of the operations*/
std::string editOperations(std::vector<std::vector<int>> matrix, std::string workingString, std::string comparisonString)
{
	int x = matrix.size() - 1;
	int y = matrix[0].size() - 1;
	std::string edits = "";
	std::string s;
	while (x != 0 || y != 0)
	{
		if (x == 0) //if x == 0 the only way to go is up so deletion by default
		{
			edits = " -" + edits;
			y -= 1;
		}
		else if (y == 0) // if y == 0 the only way to go is horizontal so insert by default
		{
			s.push_back(comparisonString[x - 1]); //turning a char into a string, prepending it to the string of edits
			edits = " +" + s + edits;
			s.clear();

			x -= 1;
		}
		else if (matrix[x - 1][y - 1] <= matrix[x - 1][y] && matrix[x - 1][y - 1] <= matrix[x][y - 1]) //moved back diagonally: a replace or keep
		{
			if (comparisonString[x - 1] == workingString[y - 1])
			{
				edits = " ^" + edits;
			}
			else
			{
				s.push_back(comparisonString[x - 1]);
				edits = " /" + s + edits;
				s.clear();
			}
			x -= 1;
			y -= 1;
		}
		else if (matrix[x - 1][y] <= matrix[x - 1][y - 1] && matrix[x - 1][y] <= matrix[x][y - 1]) //moved back horizontally: insert
		{
			s.push_back(comparisonString[x - 1]);
			edits = " +" + s + edits;
			s.clear();
			x -= 1;
		}
		else if (matrix[x][y - 1] <= matrix[x - 1][y] && matrix[x][y - 1] <= matrix[x - 1][y - 1]) //moved vertically: deletion
		{
			edits = " -" + edits;
			y -= 1;
		}
	}
	return edits;
}
