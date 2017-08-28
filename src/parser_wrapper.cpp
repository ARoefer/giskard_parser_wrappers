#include <giskard_sim/plugin_interfaces.h>
#include <giskard_suturo_parser/giskard_parser.hpp>
#include <giskard_suturo_parser/parser.h>
#include <giskard_suturo_parser/utils.h>

namespace giskard_parser_wrapper {
class ParserWrapper : public giskard_sim::IGiskardParser {
public:
    std::string getName() const { return "Giskard parser v1.0"; }
    std::string getFileSuffix() const { return "giskard"; }
    giskard_core::QPControllerSpec loadFromFile(const std::string& path) {
        std::ifstream t(path);
        std::string fileStr;
        t.seekg(0, std::ios::end);   
        fileStr.reserve(t.tellg());
        t.seekg(0, std::ios::beg);
        fileStr.assign((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
        giskard_core::GiskardLangParser glParser;
        return glParser.parseQPController(fileStr);
    }
};

class GPPWrapper : public giskard_sim::IGiskardParser {
public:
    std::string getName() const { return "Giskard++ parser"; }
    std::string getFileSuffix() const { return "gpp"; }
    giskard_core::QPControllerSpec loadFromFile(const std::string& path) {
        giskard_suturo::GiskardPPParser parser;
        giskard_core::QPControllerSpec qpSpec = parser.parseFromFile(path);
        cout << "SCOPE: --------------------------------------------" << endl;
        for (size_t i = 0; i < qpSpec.scope_.size(); i++) {
            std::string specStr = qpSpec.scope_[i].name + " = ";
            giskard_suturo::specToString(specStr, qpSpec.scope_[i].spec);
            cout << '\t' << specStr << endl;
        }
        cout << "CONTROLLABLES: --------------------------------------------" << endl;
        for (size_t i = 0; i < qpSpec.controllable_constraints_.size(); i++){
            std::string specStr;
            giskard_suturo::specToString(specStr, qpSpec.controllable_constraints_[i]);
            cout << '\t' << specStr << endl;
        }

        cout << "SOFTS: --------------------------------------------" << endl;
        for (size_t i = 0; i < qpSpec.soft_constraints_.size(); i++){
            std::string specStr;
            giskard_suturo::specToString(specStr, qpSpec.soft_constraints_[i]);
            cout << '\t' << specStr << endl;
        }

        cout << "HARDS: --------------------------------------------" << endl;
        for (size_t i = 0; i < qpSpec.hard_constraints_.size(); i++){
            std::string specStr;
            giskard_suturo::specToString(specStr, qpSpec.hard_constraints_[i]);
            cout << '\t' << specStr << endl;
        }
        cout << "---------------------------------------------------" << endl;

        return qpSpec;
    }
};

}

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(giskard_parser_wrapper::ParserWrapper, giskard_sim::IGiskardParser)
PLUGINLIB_EXPORT_CLASS(giskard_parser_wrapper::GPPWrapper, giskard_sim::IGiskardParser)