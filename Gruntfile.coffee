module.exports = (grunt) ->
  grunt.initConfig
    pkt: grunt.file.readJSON 'package.json'

    coffee:
      glob_to_multiple:
        expand: true
        cwd: 'src'
        src: ['**/*.coffee']
        dest: 'lib'
        ext: '.js'

    coffeelint:
      options:
        no_empty_param_list:
          level: 'error'
        max_line_length:
          level: 'ignore'
        indentation:
          level: 'ignore'

      src: ['src/**/*.coffee']
      test: ['spec/**/*.coffee']
      gruntfile: ['Gruntfile.coffee']

    cpplint:
      files: ['src/**/*.cc']
      reporter: 'spec'
      verbosity: 1
      filters:
        build:
          include: false
          namespace: false
        legal:
          copyright: false
        readability:
          braces: false

    shell:
      rebuild:
        command: 'npm build .'
        options:
          stdout: true
          stderr: true
          failOnError: true

      clean:
        command: 'rm -rf build lib'
        options:
          stdout: true
          stderr: true
          failOnError: true

      test:
        command: 'npm test'
        options:
          stdout: true
          stderr: true
          failOnError: true

  grunt.loadNpmTasks 'grunt-contrib-coffee'
  grunt.loadNpmTasks 'grunt-shell'
  grunt.loadNpmTasks 'grunt-coffeelint'
  grunt.loadNpmTasks 'node-cpplint'

  grunt.registerTask 'default', ['coffee', 'lint', 'shell:rebuild']
  grunt.registerTask 'lint', ['coffeelint', 'cpplint']
  grunt.registerTask 'test', ['coffee', 'lint', 'shell:test']
  grunt.registerTask 'clean', ['shell:clean']
