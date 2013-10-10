#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>


using namespace std;


namespace bptree = boost::property_tree;

using namespace boost::property_tree;


int main(int, char**)
{
  bptree::ptree pt;
  
  try
  {
  	bptree::read_xml("test.xml", pt);
  	
  	// /*bptree::ptree::value_type& v =*/ pt.get_child("subModels");

  	/*BOOST_FOREACH(bptree::ptree::value_type& v, pt.get_child("subModels"))
  	{
  		const std::string& meshName = v.second.get<std::string>("mesh");
  		const std::string& materialName = v.second.get<std::string>("material");
  		const std::string& dpMaterialName = v.second.get<std::string>("dpMaterial");


  		cout << meshName << " " << materialName << " " << dpMaterialName << " " << endl;
  	}

  	boost::optional<std::string> skelName = pt.get_optional<std::string>("skeleton");

  	if(skelName.is_initialized())
  	{
  		cout << skelName.get() << endl;
  	}

  	boost::optional<bptree::ptree&> skelAnims = pt.get_child_optional("skelAnims");
  	if(skelAnims.is_initialized())
  	{
  		BOOST_FOREACH(bptree::ptree::value_type& v, skelAnims.get())
  		{
  			const std::string& dpMaterialName = v.second.data();
  			cout << dpMaterialName << endl;
  		}
  	}

	  //cout << pt.get_child("subModels").size() << endl;*/
	  
		ptree::iterator it = pt.begin();
		for(; it != pt.end(); it++)
		{
			cout << (*it).first << endl;
		}
	}
	catch(std::exception& e)
	{
		cerr << "Error: " << e.what() << endl;
	}

	return 0;
}
