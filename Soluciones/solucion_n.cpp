#include <iostream>
#include <string.h>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <fstream>
using namespace std;
using namespace std::chrono;

int n, m, k;
vector<vector<vector<string>>> show;
unordered_map<string, int> animals;
unordered_map<string, int> animalParticipations;
int showAwesomeness = 0;

/**
 * Reads the input related to the animals from an
 * specific test case in the directory Pruebas.
 * 
 * @param file ifstream --> Test case file.
 */
void setAnimals(ifstream & file) {
	
	string name;
	int awesomeness;
	for(int i=0; i<n; i++){
		file >> name >> awesomeness;
		animals[name] = awesomeness;
	}
}

/**
 * Reads the input related to the show from an
 * specific test case in the directory Pruebas.
 *
 * @param file ifstream --> Test case file.
 */
void setShow(ifstream & file){
	
	vector<vector<string>> part;
	vector<string> scene;
	string animal;

	for(int i=0; i<k*(m-1); i++){
		scene.clear();

		file>>animal;
		scene.push_back(animal);
		animalParticipations[animal]++;
		file>>animal;
		scene.push_back(animal);
		animalParticipations[animal]++;
		file>>animal;
		scene.push_back(animal);
		animalParticipations[animal]++;

		part.push_back(scene);
	}

	show.push_back(part);
	
	for(int i=1; i<m; i++){
		part.clear();
		
		for(int j=0; j<k; j++){
			scene.clear();
			
			file>>animal;
			scene.push_back(animal);
			animalParticipations[animal]++;
			file>>animal;
			scene.push_back(animal);
			animalParticipations[animal]++;
			file>>animal;
			scene.push_back(animal);
			animalParticipations[animal]++;

			part.push_back(scene);
		}
		show.push_back(part);
	}
}

/**
 * Gets the awesomeness of on scene
 * 
 * @param scene vector<string> --> vector representing
 * a part's scene.
 * @return result int --> Value of the awesomeness of
 * the scene.
 */
int getSceneAwesomeness(vector<string> scene){

	int result = animals[scene[0]] + animals[scene[1]] + animals[scene[2]];

	return result;
}

/**
 * Gets the awesomeness of a part
 * 
 * @param part vector<vector<string>> --> vector
 * representing a show's part.
 * @return result int --> Value of the awesomeness of
 * the part.
 */
int getPartAwesomeness(vector<vector<string>> part){

	int result = 0;
    
	for(int i=0; i<part.size(); i++)
		result += getSceneAwesomeness(part[i]);
	
	return result;
}

/**
 * Sorts the animals of each scene of each part of the show.
 * 
 */
void sortAnimals() {
	
	vector<vector<string>> part;
	vector<string> scene;

	for(int i=0; i<m; i++) {

		part = show[i];
		for(int j=0; j<part.size(); j++) {

			scene = part[j];
			int awesomenessFirstAnimal = animals[scene[0]];
			int awesomenessSecondAnimal = animals[scene[1]];
			int awesomenessThirdAnimal = animals[scene[2]];
			
			if(awesomenessFirstAnimal < awesomenessSecondAnimal) {
			
				if(awesomenessSecondAnimal < awesomenessThirdAnimal) {
				
					continue;
	
				} else if(awesomenessFirstAnimal < awesomenessThirdAnimal) {
				
					swap(show[i][j][1], show[i][j][2]);
				} else {
					
					swap(show[i][j][1], show[i][j][2]);
					swap(show[i][j][0], show[i][j][1]);
				}
			} else if(awesomenessFirstAnimal < awesomenessThirdAnimal) {
			
				swap(show[i][j][0], show[i][j][1]);
	
			} else if(awesomenessSecondAnimal < awesomenessThirdAnimal) {
				
				swap(show[i][j][0], show[i][j][2]);
				swap(show[i][j][0], show[i][j][1]);
	
			} else {
				
				swap(show[i][j][0], show[i][j][2]);
			}
		}
	}
}

/**
 * Sorts the scenes of each part of the show by counting sort.
 * 
 */
void sortScenes() {

	for(int i=0; i<m; i++) {
		
		vector<vector<string>> part = show[i];
		vector<int> sortedIndexes[3*n-2];

		for(int j=0; j<part.size(); j++) {

			vector<string> scene = part[j];
			int awesomeness = getSceneAwesomeness(scene);
			sortedIndexes[awesomeness].push_back(j);
		}
		
		vector<vector<string>> sortedPart;
		for(int j=6; j<=3*n-3; j++) {
			
			vector<int> scenesWithEqualAwesomeness = sortedIndexes[j];

			for(int l=0; l<scenesWithEqualAwesomeness.size(); l++) {
				sortedPart.push_back(part[scenesWithEqualAwesomeness[l]]);
			}	
		}
		show[i] = sortedPart;
	}
}

/**
 * Sorts the parts of the show.
 * 
 */
void sortParts() {

	int partAwesomeness[m];
	int biggestPartAwesomeness = 0;

	for(int i=0; i<m; i++) {
		
		int awesomeness = getPartAwesomeness(show[i]);
		partAwesomeness[i] = awesomeness;
		showAwesomeness += awesomeness;
		
		if(awesomeness > biggestPartAwesomeness)
			biggestPartAwesomeness = awesomeness;
	}

	vector<int> sortedIndexes[biggestPartAwesomeness+1];

	for(int i=1; i<m; i++) {
		
		vector<vector<string>> part = show[i];
		int awesomeness = partAwesomeness[i];

		sortedIndexes[awesomeness].push_back(i);
	}

	vector<vector<vector<string>>> sortedShow;
	sortedShow.push_back(show[0]);

	for(int i=6; i<=biggestPartAwesomeness; i++) {
		
		vector<int> partsWithEqualAwesomeness = sortedIndexes[i];

		for(int j=0; j<partsWithEqualAwesomeness.size(); j++) {
			sortedShow.push_back(show[partsWithEqualAwesomeness[j]]);
		}	
	}

	show = sortedShow;
}

/**
 * Main function
 *
 */
int main() {
    auto start = high_resolution_clock::now();
	string testCase = "prueba1"
                   "";
	ifstream file ("../Pruebas/" + testCase + ".txt");
	file >> n >> m >> k;
	setAnimals(file);
	setShow(file);
	sortAnimals();
	//sortScenes();
	sortParts();

	//Pruebas

	cout << endl << "Apertura" << endl;

	for(int i=0; i<k*(m-1); i++) {
		for(int j=0; j<3; j++)
			cout << show[0][i][j] << " ";
		cout << endl;
	}

	cout << endl;

	for(int i=1; i<m; i++) {
		cout << "Parte " << i << endl;
		for(int j=0; j<k; j++) {
			for(int l=0; l<3; l++)
				cout << show[i][j][l] << " ";
			cout << endl;
		}

		cout << endl;
	}

	unordered_map<string, int>::iterator it = animalParticipations.begin();
	int biggestQuantityParticipations = 0;
	int smallestQuantityParticipations = (m - 1) * 2 * k;
	vector<string> popularAnimals;
	vector<string> unpopularAnimals;

	while (it != animalParticipations.end())
	{

		int participations = it->second;

		if(participations > biggestQuantityParticipations) {
			biggestQuantityParticipations = participations;
			popularAnimals.clear();
			popularAnimals.push_back(it->first);

		} else if(participations == biggestQuantityParticipations) {
			popularAnimals.push_back(it->first);
		}

		if(participations < smallestQuantityParticipations) {
			smallestQuantityParticipations = participations;
			unpopularAnimals.clear();
			unpopularAnimals.push_back(it->first);

		} else if(participations == smallestQuantityParticipations) {
			unpopularAnimals.push_back(it->first);
		}

		it++;
	}

	int numberOfAnimals = popularAnimals.size();
	bool plural = numberOfAnimals > 1;

	if(plural) {

		string message = "";
		message += popularAnimals[0];
		int i = 1;
		
		for(int i = 1; i<numberOfAnimals - 1; i++) {
			message += ", " + popularAnimals[i];
		}
		message += " y " + popularAnimals[i];
		
		cout << "Los animales que más participaron fueron: " + message + " con " + to_string(biggestQuantityParticipations) + " participaciones." << endl;
	} else {
		cout << "El animal que más participó fue: " + popularAnimals[0] + " con " + to_string(biggestQuantityParticipations) + " participaciones." << endl;
	}

	numberOfAnimals = unpopularAnimals.size();
	plural = numberOfAnimals > 1;

	if(plural) {

		string message = "";
		message += unpopularAnimals[0];
		int i = 1;
		
		for(int i; i<numberOfAnimals - 1; i++) {
			message += ", " + unpopularAnimals[i];
		}
		message += " y " + unpopularAnimals[i];
		
		cout << "Los animales que menos participaron fueron: " + message + " con " + to_string(smallestQuantityParticipations) + " participaciones." << endl;
	} else {
		
		cout << "El animal que más participó fue: " + unpopularAnimals[0] + " con " + to_string(smallestQuantityParticipations) + " participaciones." << endl;
	}
	
	cout << "El promedio de grandeza de todo el espectaculo fue de " << fixed << setprecision(2) << showAwesomeness / ((m - 1.0) * k * 2) << endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tiempo: "<< duration.count() << endl;
	return 0;

}
