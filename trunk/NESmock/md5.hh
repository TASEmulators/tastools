#include <string>

/* Returns a 16-byte binary string. */
const std::string MD5sum(const std::string& data);

/* Returns a 16-byte hex string with lowercase. */
const std::string MD5sumHex(const std::string& data);

/* Returns a 16-byte hex string with uppercase. */
const std::string MD5sumHexUp(const std::string& data);
