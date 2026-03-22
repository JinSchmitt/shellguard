
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
bool isDangerous(const string& input) {
    vector<string> dangerousCommands = {
        "rm -rf",
   "rm -rf /",
   "rm -rf *",
   "dd",
   "mkfs",
   ":(){:|:&};:",
   "chmod -R 777",
   "chmod -R 777 /",
   "chown -R",
   "mv /* /dev/null",
   "> /dev/sda",
   "dd if=/dev/random",
   "curl | sh",
   "wget | sh"
    };

    for (const string& command : dangerousCommands) {
        if (input.find(command) != string::npos) {
            return true;
        }

    }
    return false;
}