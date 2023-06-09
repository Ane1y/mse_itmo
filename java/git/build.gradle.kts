plugins {
    java
    application
}

group = "org.example"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
    implementation("org.jetbrains:annotations:16.0.2")
    implementation("commons-cli:commons-cli:1.5.0")
    implementation("commons-io:commons-io:2.12.0")
    implementation("com.squareup:javapoet:1.13.0")
    testImplementation(platform("org.junit:junit-bom:5.7.0"))
    testImplementation("org.junit.jupiter:junit-jupiter")
    testImplementation("org.easytesting:fest-assert-core:2.0M10")
}

java {
    sourceSets {
        main {
            java.setSrcDirs(listOf("src/main"))
            resources.setSrcDirs(listOf("src/resources"))
        }
        test {
            java.setSrcDirs(listOf("src/test"))
        }
    }
}

tasks.withType<JavaCompile>().configureEach {
    options.encoding = "UTF-8"
    options.isDeprecation = true
    options.release.set(17)
    // options.compilerArgs.add("-Xlint:unchecked")
}
tasks.test {
    useJUnitPlatform()
}
