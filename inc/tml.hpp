#ifndef __tml__
#define __tml__

#include "jsonz.hpp"
#include "agent.hpp"
#include "pattern.hpp"
#include <map>

namespace gentml {
using namespace alioth;
using namespace std;
using strings = chainz<string>;

/**
 * @struct Tml : tmLanguage
 * @desc :
 *  描述一个Tml配置
 */
struct Tml {

    public:

        /**
         * @member name : 配置名 */
        string name;

        /**
         * @member scopeName : 语法定义范围 */
        string scopeName;

        /**
         * @member fileTypes : 文件类型 */
        strings fileTypes;

        /**
         * @member patterns : 模式 */
        $patterns patterns;

        /**
         * @member repository : 仓库 */
        map<string, $patterns> repository;

    public:

        /**
         * @operator json : 将配置转化为json
         */
        operator json()const;
};

}
#endif