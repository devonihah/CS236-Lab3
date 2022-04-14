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
		cout << "Query Evaluation" << endl;
		vector<predicate> datalogQueries = myDatalog.getQueries();
		for (unsigned int i = 0; i < datalogQueries.size(); i++)
		{
			relation relationsToGrab, columnsToGrab;
			relationsToGrab = myDatabase.findRelation(datalogQueries[i].getPredicateName());
			vector<int> Positions;
			vector<string> renameVals;

			for (unsigned int j = 0; j < datalogQueries[i].getVector().size(); j++)
			{
				int nextParameterToSelect = j + 1;
				parameter nextParameterToCheck = datalogQueries[i].getVector()[j];
				if (!nextParameterToCheck.getIsID())
					relationsToGrab = relationsToGrab.select(nextParameterToSelect, nextParameterToCheck.toString());
			}

			for (unsigned int j = 0; j < datalogQueries[i].getVector().size(); j++)
			{
				for (unsigned int k = j + 1; k < datalogQueries[i].getVector().size(); k++)
				{
					bool exitLoop = false;
					if (datalogQueries[i].getVector()[j].toString() == datalogQueries[i].getVector()[k].toString())
					{
						relationsToGrab = relationsToGrab.select(j, k);
						exitLoop = true;
					}
					if (exitLoop) break;
				}
			}

			for (unsigned int j = 0; j < datalogQueries[i].getVector().size(); j++)
			{
				parameter nextParameterToCheck = datalogQueries[i].getVector()[j];
				if (nextParameterToCheck.getIsID())
				{
					bool duplicate = false;
					for (unsigned int x = 0; x < Positions.size(); x++)
					{
						if (nextParameterToCheck.toString() == renameVals[x])
						{
							duplicate = true;
						}
						if (duplicate)
						{
							break;
						}
					}
					if (!duplicate)
					{
						Positions.push_back(j);
						renameVals.push_back(nextParameterToCheck.toString());
					}

				}
			}

			columnsToGrab = relationsToGrab.project(Positions);

			scheme nextScheme;
			nextScheme.setAttributes(renameVals);
			columnsToGrab.setScheme(nextScheme);

			cout << datalogQueries[i].toString() << "? ";
			if (relationsToGrab.tuplesSetSize() == 0)
			{
				cout << "No\n";
			}
			else
			{
				cout << "Yes(" << columnsToGrab.tuplesSetSize() << ")\n";
				cout << columnsToGrab.toString();
			}
		}
	}
};