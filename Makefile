.PHONY: all
all: docker.build opentracing/src/json.hpp

opentracing/src/json.hpp:
	wget --quiet --output-document=$@ https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp

.PHONY: docker.build
docker.build: test
	docker build -f Dockerfile -t opentracing-contrib/nginx-opentracing .

.PHONY: test
test:
	./ci/system_testing.sh

.PHONY: clean
clean:
	rm -fr test-log
