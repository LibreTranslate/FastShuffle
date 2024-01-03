
#include "vendor/cxxopts.hpp"
#include "shuffle.hpp"

int main(int argc, char **argv) {
    cxxopts::Options options("shuffle", "Quickly shuffle parallel corpora");
    options.add_options()
        ("s,src", "Input source corpus", cxxopts::value<std::string>())
        ("t,tgt", "Input target corpus", cxxopts::value<std::string>())
        ("h,help", "Print usage")
        ;
    options.parse_positional({ "src", "tgt" });
    options.positional_help("[src] [tgt]");

    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    if (result.count("help") || !result.count("src") || !result.count("tgt")) {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    try {
        const auto source = result["src"].as<std::string>();
        const auto target = result["tgt"].as<std::string>();
        
        auto result = shuffle(source, target);
        std::cout << "W\t" << std::get<0>(result) << std::endl;
        std::cout << "W\t" << std::get<1>(result) << std::endl;
    }catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
