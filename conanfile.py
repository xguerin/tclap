from conans import ConanFile, tools

class TclapConan(ConanFile):
    name = "tclap"
    version = "1.2.3"
    license = "MIT License"
    author = "Michael E. Smoot"
    url = "http://github.com/xguerin/tclap"
    description = "Templatized Command Line Argument Parser"
    topics = ("c++", "commandline parser")
    no_copy_source = True
    exports_sources = ["include/*"]

    def source(self):
        pkg_name = "v" + self.version + ".zip"
        tools.download(self.url + "/archive/" + pkg_name, pkg_name)
        tools.unzip(pkg_name)

    def package(self):
        src_folder = self.name + "-" + self.version
        self.copy("*.h", "include", src_folder + "/include")
