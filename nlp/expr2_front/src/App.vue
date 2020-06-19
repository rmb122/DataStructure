<template>
  <div id="app">
    <Index></Index>
  </div>
</template>

<script>
  import Index from '@/components/Index';

  export default {
    name: 'app',
    components: {
      Index
    }
  }
</script>

<style>
  #app {
    font-family: 'Avenir', Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
    background-color: white !important;
    width: 100%;
    height: 100%;
    position: absolute;
  }
</style>
