#pragma once

#include <set>
#include <vector>
#include "rules.h"
#include "database.h"
#include "datalog.h"
#include "predicate.h"
#include "relation.h"
#include <iostream>


class interpreter {
private:
	database myDatabase;
	datalog myDatalog;
public:
	interpreter(datalog newDatalog)
	{
		myDatalog = newDatalog;
		storeData();
		interpretQueries();
	}

	void storeData()
	{
		for (size_t i = 0; i < myDatalog.getSchemes().size(); i++)
		{
			scheme newScheme(myDatalog.getSchemes().at(i).getParamsString());
			relation newRelation(myDatalog.getSchemes().at(i).getPredicateName(), newScheme);
			myDatabase.addRelation(newRelation);
		}
		for (size_t i = 0; i < myDatalog.getFacts().size(); i++)
		{
			tuples newTuple(myDatalog.getFacts()[i].getParamsString());
			myDatabase.addTupleToRelation(myDatalog.getFacts()[i].getPredicateName(),newTuple);
		}
	}

	void interpretQueries()
	{
		vector<predicate> queries = myDatalog.getQueries();

		for (size_t i = 0; i < queries.size(); i++)
		{
			string newID = queries.at(i).getPredicateName();
			vector<int> intVector;
			vector<string> stringVector;
			relation newRelation = myDatabase.findRelation(queries[i].getPredicateName());

			for (size_t j = 0; j < queries.at(i).getVector().size(); j++)
			{
				parameter currentParameter = queries.at(i).getVector().at(j);
				if (!currentParameter.getIsID())
				{
					newRelation = newRelation.select((int)j, currentParameter.toString());
				}
				else
				{
					bool repeatRelation = false;
					for (size_t k = 0; k < stringVector.size(); k++)
					{
						if (stringVector.size() != 0 && stringVector.at(k) == currentParameter.toString())
						{
							repeatRelation = true;
							newRelation = newRelation.select(int(j), int(k));
						}
					}
					if (repeatRelation == false)
					{
						stringVector.push_back(currentParameter.toString());
						intVector.push_back(int(j));
					}
				}
			}

			relation finalRelation;

			finalRelation = newRelation.project(intVector);

			newRelation = finalRelation.rename(stringVector);
			//rename using indices

			cout << queries.at(i).toString() << "? ";
			if (newRelation.tuplesSetSize() == 0)
			{
				cout << "No\n";
			}
			else
			{
				cout << "Yes(" << newRelation.tuplesSetSize() << ")\n";
				cout << newRelation.toString();
			}
		}
	}
};