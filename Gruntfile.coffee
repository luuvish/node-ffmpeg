module.exports = (grunt) ->
  grunt.initConfig
    pkt: grunt.file.readJSON 'package.json'

    coffee:
      glob_to_multiple:
        expand: true
        cwd: 'src/coffee'
        src: ['**/*.coffee']
        dest: 'dist/lib'
        ext: '.js'

    coffeelint:
      options:
        no_empty_param_list:
          level: 'error'
        max_line_length:
          level: 'ignore'
        #indentation:
        #  level: 'ignore'

      src: ['src/coffee/**/*.coffee']
      test: ['spec/**/*.coffee']
      gruntfile: ['Gruntfile.coffee']

    cpplint:
      files: ['src/addons/**/*.cc', 'src/addons/**/*.h']
      reporter: 'spec'
      verbosity: 1
      filters:
        build:
          include: false
          namespace: false ##
        legal:
          copyright: false
        readability:
          braces: false
        runtime:
          references: false
          sizeof: false
        whitespace:
          line_length: false

    shell:
      rebuild:
        command: 'npm build .'
        options:
          stdout: true
          stderr: true
          failOnError: true

      test:
        command: 'node node_modules/jasmine-tagged/bin/jasmine-tagged --captureExceptions --coffee spec/'
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
  grunt.registerTask 'test', ['default', 'shell:test']
  grunt.registerTask 'prepublish', ['clean', 'coffee', 'lint']
  grunt.registerTask 'clean', ->
    rm = require('rimraf').sync
    rm 'lib'
